#include "debtpilot/cli/DebtFileManager.hpp"

#include "debtpilot/DebtType.hpp"
#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"

#include <nlohmann/json.hpp>

#include <charconv>
#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>


namespace debtpilot::cli
{
    namespace
    {
        using Json = nlohmann::json;

        std::int64_t parseInteger(const std::string& text)
        {
            std::int64_t value{};
            const char* begin = text.data();
            const char* end = begin + text.size();

            const auto [position, error] = std::from_chars(begin, end, value);

            if(error != std::errc{} || position != end)
            {
                throw std::invalid_argument{"Invalid whole-number value: " + text};
            }
            return value;
        }

        Money parseMoney(const std::string& text)
        {
            if(text.empty())
            {
                throw std::invalid_argument{"Money value cannot be empty"};
            }

            if(text.front() == '-')
            {
                throw std::invalid_argument{"Money value cannot be negative"};
            }

            const std::size_t decimalPosition = text.find('.');

            if(decimalPosition != std::string::npos && text.find('.', decimalPosition + 1) != std::string::npos)
            {
                throw std::invalid_argument{"Money value contains multiple decimal points"};
            }

            const std::string rupeesText = decimalPosition == std::string::npos ? text : text.substr(0, decimalPosition);
            std::string paiseText = decimalPosition == std::string::npos ? "" : text.substr(decimalPosition + 1);

            if(rupeesText.empty() || paiseText.size() > 2)
            {
                throw std::invalid_argument{"Invalid money value: " + text};
            }

            if(paiseText.empty())
            {
                paiseText = "00";
            }
            else if(paiseText.size() == 1)
            {
                paiseText.push_back('0');
            }

            const std::int64_t rupees = parseInteger(rupeesText);
            const std::int64_t paise = parseInteger(paiseText);

            if(rupees>(std::numeric_limits<std::int64_t>::max()-paise)/ 100)
            {
                throw std::overflow_error{"Money value is too large"};
            }
            return Money::fromPaise(rupees * 100 + paise);
        }

        DebtType parseDebtType(const std::string& type)
        {
            if(type == "credit_card")
            {
                return DebtType::CreditCard;
            }
            throw std::invalid_argument{"Unsupported debt type: " + type};
        }
    }

    void DebtFileManager::generateTemplate(std::size_t debtCount, const std::filesystem::path& outputPath)
    {
        if(debtCount == 0)
        {
            throw std::invalid_argument{"Debt count must be greater than zero"};
        }

        Json document{
            {"monthly_extra_payment_rupees", "0.00"},
            {"maximum_months", 1200},
            {"debts", Json::array()}
        };

        for(std::size_t index = 0; index < debtCount; ++index)
        {
            document["debts"].push_back({
                {"id", "debt-" + std::to_string(index +1)},
                {"name", "Debt " + std::to_string(index +1)},
                {"type", "credit_card"},
                {"outstanding_balance_rupees", "0.00"},
                {"annual_interest_basis_points", 0},
                {"minimum_payment_rupees", "0.00"}
            });
        }

        std::ofstream output{outputPath};

        if(!output)
        {
            throw std::runtime_error{"Could not create file: " + outputPath.string()};
        }
        output << document.dump(4)<< '\n';
    }

    DebtFileData DebtFileManager::load(const std::filesystem::path& inputPath)
    {
        std::ifstream input{inputPath};

        if(!input)
        {
            throw std::runtime_error{"Could not open file: " + inputPath.string()};
        }

        Json document;

        try
        {
            input >> document;
        }
        catch(const Json::exception& exception)
        {
            throw std::invalid_argument{std::string{"Invalid JSON: "} + exception.what()};
        }

        const Money monthlyExtraPayment = parseMoney(document.at("monthly_extra_payment_rupees").get<std::string>());
        const std::size_t maximumMonths = document.at("maximum_months").get<std::size_t>();
        if(maximumMonths == 0)
        {
            throw std::invalid_argument{"maximum_months must be greater than zero"};
        }
        
        const Json& debtEntries = document.at("debts");

        if(!debtEntries.is_array() || debtEntries.empty())
        {
            throw std::invalid_argument{"debts must contain at least one debt"};
        }

        std::vector<Debt> debts;
        debts.reserve(debtEntries.size());

        for(const Json& entry : debtEntries)
        {
            const std::string id = entry.at("id").get<std::string>();
            const std::string name = entry.at("name").get<std::string>();
            const DebtType type = parseDebtType(entry.at("type").get<std::string>());
            const Money balance = parseMoney(entry.at("outstanding_balance_rupees").get<std::string>());
            const std::int32_t basisPoints = entry.at("annual_interest_basis_points").get<std::int32_t>();
            const Money minimumPayment = parseMoney(entry.at("minimum_payment_rupees").get<std::string>());

            debts.emplace_back(id, name, type, balance, InterestRate::fromBasisPoints(basisPoints), minimumPayment);
        }

        return DebtFileData(std::move(debts), monthlyExtraPayment, maximumMonths);
    
    }


}