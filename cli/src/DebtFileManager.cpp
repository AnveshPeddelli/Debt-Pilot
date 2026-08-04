#include "debtpilot/cli/DebtFileManager.hpp"

#include "debtpilot/DebtType.hpp"
#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"

#include <nlohmann/json.hpp>

#include <charconv>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unordered_set>
#include <utility>
#include <vector>

namespace debtpilot::cli
{

namespace
{

using Json = nlohmann::json;

constexpr std::uint32_t CurrentSchemaVersion = 1;
constexpr std::size_t DefaultMaximumMonths = 1'200;

// ------------------------------------------------------------
// Integer parsing
// ------------------------------------------------------------

[[nodiscard]] std::int64_t parseInteger(const std::string& text)
{
    if (text.empty())
    {
        throw std::invalid_argument{
            "Integer value cannot be empty"
        };
    }

    std::int64_t value{};

    const char* begin = text.data();
    const char* end = begin + text.size();

    const auto [position, error] =
        std::from_chars(begin, end, value);

    if (error == std::errc::result_out_of_range)
    {
        throw std::overflow_error{
            "Integer value is too large: " + text
        };
    }

    if (error != std::errc{} || position != end)
    {
        throw std::invalid_argument{
            "Invalid whole-number value: " + text
        };
    }

    return value;
}

// ------------------------------------------------------------
// Money parsing
//
// Accepted:
// "0"
// "0.00"
// "1500"
// "1500.5"
// "1500.50"
//
// Rejected:
// ""
// "-100"
// "12..50"
// "1.234"
// "abc"
// ------------------------------------------------------------

[[nodiscard]] Money parseMoney(const std::string& text)
{
    if (text.empty())
    {
        throw std::invalid_argument{
            "Money value cannot be empty"
        };
    }

    if (text.front() == '-')
    {
        throw std::invalid_argument{
            "Money value cannot be negative: " + text
        };
    }

    const std::size_t decimalPosition =
        text.find('.');

    if (
        decimalPosition != std::string::npos &&
        text.find('.', decimalPosition + 1) !=
            std::string::npos
    )
    {
        throw std::invalid_argument{
            "Money value contains multiple decimal points: " +
            text
        };
    }

    const std::string rupeesText =
        decimalPosition == std::string::npos
            ? text
            : text.substr(0, decimalPosition);

    std::string paiseText =
        decimalPosition == std::string::npos
            ? std::string{}
            : text.substr(decimalPosition + 1);

    if (rupeesText.empty())
    {
        throw std::invalid_argument{
            "Money value must contain a rupee amount: " +
            text
        };
    }

    if (paiseText.size() > 2)
    {
        throw std::invalid_argument{
            "Money value cannot contain more than "
            "two decimal places: " +
            text
        };
    }

    if (paiseText.empty())
    {
        paiseText = "00";
    }
    else if (paiseText.size() == 1)
    {
        paiseText.push_back('0');
    }

    const std::int64_t rupees =
        parseInteger(rupeesText);

    const std::int64_t paise =
        parseInteger(paiseText);

    if (rupees < 0 || paise < 0 || paise > 99)
    {
        throw std::invalid_argument{
            "Invalid money value: " + text
        };
    }

    constexpr std::int64_t PaisePerRupee = 100;

    if (
        rupees >
        (
            std::numeric_limits<std::int64_t>::max() -
            paise
        ) /
            PaisePerRupee
    )
    {
        throw std::overflow_error{
            "Money value is too large: " + text
        };
    }

    return Money::fromPaise(
        rupees * PaisePerRupee + paise
    );
}

// ------------------------------------------------------------
// JSON field helpers
// ------------------------------------------------------------

[[nodiscard]] const Json& requireField(const Json& object, const std::string& fieldName)
{
    if (!object.contains(fieldName))
    {
        throw std::invalid_argument{
            "Missing required field: " + fieldName
        };
    }

    return object.at(fieldName);
}

[[nodiscard]] std::string readRequiredString(const Json& object, const std::string& fieldName)
{
    const Json& value =
        requireField(object, fieldName);

    if (!value.is_string())
    {
        throw std::invalid_argument{
            fieldName + " must be a string"
        };
    }

    return value.get<std::string>();
}

[[nodiscard]] std::int32_t readNonNegativeInt32(const Json& object, const std::string& fieldName)
{
    const Json& value =
        requireField(object, fieldName);

    if (!value.is_number_integer())
    {
        throw std::invalid_argument{
            fieldName + " must be an integer"
        };
    }

    std::int64_t parsedValue{};

    try
    {
        parsedValue = value.get<std::int64_t>();
    }
    catch (const Json::exception&)
    {
        throw std::invalid_argument{
            fieldName + " is outside the supported range"
        };
    }

    if (parsedValue < 0)
    {
        throw std::invalid_argument{
            fieldName + " cannot be negative"
        };
    }

    if (
        parsedValue >
        std::numeric_limits<std::int32_t>::max()
    )
    {
        throw std::overflow_error{
            fieldName + " is too large"
        };
    }

    return static_cast<std::int32_t>(
        parsedValue
    );
}

[[nodiscard]] std::size_t readPositiveSize(const Json& object, const std::string& fieldName)
{
    const Json& value =
        requireField(object, fieldName);

    if (!value.is_number_integer())
    {
        throw std::invalid_argument{
            fieldName + " must be an integer"
        };
    }

    if (value.is_number_unsigned())
    {
        const std::uint64_t parsedValue =
            value.get<std::uint64_t>();

        if (parsedValue == 0)
        {
            throw std::invalid_argument{
                fieldName +
                " must be greater than zero"
            };
        }

        if (
            parsedValue >
            std::numeric_limits<std::size_t>::max()
        )
        {
            throw std::overflow_error{
                fieldName + " is too large"
            };
        }

        return static_cast<std::size_t>(
            parsedValue
        );
    }

    const std::int64_t parsedValue =
        value.get<std::int64_t>();

    if (parsedValue <= 0)
    {
        throw std::invalid_argument{
            fieldName + " must be greater than zero"
        };
    }

    if (
        static_cast<std::uint64_t>(parsedValue) >
        std::numeric_limits<std::size_t>::max()
    )
    {
        throw std::overflow_error{
            fieldName + " is too large"
        };
    }

    return static_cast<std::size_t>(
        parsedValue
    );
}

[[nodiscard]] Money readMoney(const Json& object, const std::string& fieldName)
{
    const std::string text =
        readRequiredString(object, fieldName);

    try
    {
        return parseMoney(text);
    }
    catch (const std::exception& exception)
    {
        throw std::invalid_argument{
            fieldName + ": " + exception.what()
        };
    }
}

// ------------------------------------------------------------
// Schema validation
// ------------------------------------------------------------

void validateSchemaVersion(const Json& document)
{
    const Json& schemaValue =
        requireField(document, "schema_version");

    if (!schemaValue.is_number_integer())
    {
        throw std::invalid_argument{
            "schema_version must be an integer"
        };
    }

    std::int64_t schemaVersion{};

    try
    {
        schemaVersion =
            schemaValue.get<std::int64_t>();
    }
    catch (const Json::exception&)
    {
        throw std::invalid_argument{
            "schema_version is outside the "
            "supported range"
        };
    }

    if (schemaVersion <= 0)
    {
        throw std::invalid_argument{
            "schema_version must be greater than zero"
        };
    }

    if (
        static_cast<std::uint64_t>(schemaVersion) !=
        CurrentSchemaVersion
    )
    {
        throw std::invalid_argument{
            "Unsupported schema version: " +
            std::to_string(schemaVersion) +
            ". Supported version: " +
            std::to_string(CurrentSchemaVersion)
        };
    }
}

// ------------------------------------------------------------
// Debt type conversion
// ------------------------------------------------------------

[[nodiscard]] DebtType parseDebtType(const std::string& type)
{
    if (type == "credit_card")
    {
        return DebtType::CreditCard;
    }

    /*
    Add these when the corresponding enum values exist
    in DebtType.hpp:

    if (type == "personal_loan")
    {
        return DebtType::PersonalLoan;
    }

    if (type == "education_loan")
    {
        return DebtType::EducationLoan;
    }

    if (type == "vehicle_loan")
    {
        return DebtType::VehicleLoan;
    }

    if (type == "home_loan")
    {
        return DebtType::HomeLoan;
    }

    if (type == "buy_now_pay_later")
    {
        return DebtType::BuyNowPayLater;
    }

    if (type == "informal_loan")
    {
        return DebtType::InformalLoan;
    }

    if (type == "other")
    {
        return DebtType::Other;
    }
    */

    throw std::invalid_argument{
        "Unsupported debt type: " + type
    };
}

} // namespace

// ------------------------------------------------------------
// Template generation
// ------------------------------------------------------------

void DebtFileManager::generateTemplate(std::size_t debtCount, const std::filesystem::path& outputPath)
{
    if (debtCount == 0)
    {
        throw std::invalid_argument{
            "Debt count must be greater than zero"
        };
    }

    Json document{
        {
            "schema_version",
            CurrentSchemaVersion
        },
        {
            "maximum_months",
            DefaultMaximumMonths
        },
        {
            "monthly_budget_rupees",
            "0.00"
        },
        {
            "debts",
            Json::array()
        }
    };

    for (
        std::size_t index = 0;
        index < debtCount;
        ++index
    )
    {
        const std::string number =
            std::to_string(index + 1);

        document["debts"].push_back(
            {
                {
                    "id",
                    "debt-" + number
                },
                {
                    "name",
                    "Debt " + number
                },
                {
                    "type",
                    "credit_card"
                },
                {
                    "outstanding_balance_rupees",
                    "0.00"
                },
                {
                    "annual_interest_basis_points",
                    0
                },
                {
                    "minimum_payment_rupees",
                    "0.00"
                }
            }
        );
    }

    std::ofstream output{outputPath};

    if (!output)
    {
        throw std::runtime_error{
            "Could not create file: " +
            outputPath.string()
        };
    }

    output << document.dump(4) << '\n';

    if (!output)
    {
        throw std::runtime_error{
            "Failed while writing file: " +
            outputPath.string()
        };
    }
}

// ------------------------------------------------------------
// File loading
// ------------------------------------------------------------

DebtFileData DebtFileManager::load(const std::filesystem::path& inputPath)
{
    std::ifstream input{inputPath};

    if (!input)
    {
        throw std::runtime_error{
            "Could not open file: " +
            inputPath.string()
        };
    }

    Json document;

    try
    {
        input >> document;
    }
    catch (const Json::parse_error& exception)
    {
        throw std::invalid_argument{
            std::string{"Invalid JSON: "} +
            exception.what()
        };
    }

    if (!document.is_object())
    {
        throw std::invalid_argument{
            "JSON root must be an object"
        };
    }

    validateSchemaVersion(document);

    const Money monthlyBudget =
        readMoney(
            document,
            "monthly_budget_rupees"
        );

    const std::size_t maximumMonths =
        readPositiveSize(
            document,
            "maximum_months"
        );

    const Json& debtEntries =
        requireField(document, "debts");

    if (!debtEntries.is_array())
    {
        throw std::invalid_argument{
            "debts must be an array"
        };
    }

    if (debtEntries.empty())
    {
        throw std::invalid_argument{
            "debts must contain at least one debt"
        };
    }

    std::vector<Debt> debts;
    debts.reserve(debtEntries.size());

    std::unordered_set<std::string> debtIds;
    debtIds.reserve(debtEntries.size());

    for (
        std::size_t index = 0;
        index < debtEntries.size();
        ++index
    )
    {
        const Json& entry =
            debtEntries[index];

        const std::string debtLabel =
            "Debt " + std::to_string(index + 1);

        try
        {
            if (!entry.is_object())
            {
                throw std::invalid_argument{
                    "entry must be an object"
                };
            }

            const std::string id =
                readRequiredString(entry, "id");

            if (id.empty())
            {
                throw std::invalid_argument{
                    "id cannot be empty"
                };
            }

            if (!debtIds.insert(id).second)
            {
                throw std::invalid_argument{
                    "Duplicate debt ID: " + id
                };
            }

            const std::string name =
                readRequiredString(entry, "name");

            if (name.empty())
            {
                throw std::invalid_argument{
                    "name cannot be empty"
                };
            }

            const DebtType type =
                parseDebtType(
                    readRequiredString(
                        entry,
                        "type"
                    )
                );

            const Money balance =
                readMoney(
                    entry,
                    "outstanding_balance_rupees"
                );

            const std::int32_t basisPoints =
                readNonNegativeInt32(
                    entry,
                    "annual_interest_basis_points"
                );

            const Money minimumPayment =
                readMoney(
                    entry,
                    "minimum_payment_rupees"
                );

            debts.emplace_back(
                id,
                name,
                type,
                balance,
                InterestRate::fromBasisPoints(
                    basisPoints
                ),
                minimumPayment
            );
        }
        catch (const std::exception& exception)
        {
            throw std::invalid_argument{
                debtLabel + ": " + exception.what()
            };
        }
    }

    return DebtFileData{
        std::move(debts),
        monthlyBudget,
        maximumMonths
    };
}

} // namespace debtpilot::cli