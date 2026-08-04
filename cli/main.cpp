#include "debtpilot/DebtPlanningRequest.hpp"
#include "debtpilot/DebtPlanningService.hpp"
#include "debtpilot/MinimumBudgetCalculator.hpp"
#include "debtpilot/Money.hpp"

#include "debtpilot/cli/ConsoleReporter.hpp"
#include "debtpilot/cli/DebtFileManager.hpp"

#include <charconv>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace
{

[[nodiscard]] std::size_t parseDebtCount(const std::string& text)
{
    if (text.empty())
    {
        throw std::invalid_argument{
            "Debt count cannot be empty"
        };
    }

    std::size_t value{};

    const char* begin = text.data();
    const char* end = begin + text.size();

    const auto [position, error] =
        std::from_chars(
            begin,
            end,
            value
        );

    if (
        error != std::errc{} ||
        position != end ||
        value == 0
    )
    {
        throw std::invalid_argument{
            "Debt count must be a positive whole number"
        };
    }

    return value;
}

void printUsage()
{
    std::cout
        << "DebtPilot CLI\n\n"
        << "Usage:\n"
        << "  debtpilot_cli generate-template "
        << "<debt-count> <output-file>\n"
        << '\n'
        << "  debtpilot_cli run <input-file>\n"
        << '\n'
        << "  debtpilot_cli run <input-file> "
        << "--details snowball\n"
        << '\n'
        << "  debtpilot_cli run <input-file> "
        << "--details avalanche\n"
        << '\n'
        << "Examples:\n"
        << "  debtpilot_cli generate-template "
        << "6 debts.json\n"
        << '\n'
        << "  debtpilot_cli run debts.json\n"
        << '\n'
        << "  debtpilot_cli run debts.json "
        << "--details avalanche\n";
}

void printMoney(
    const std::string& label,
    debtpilot::Money money
)
{
    const std::int64_t paise =
        money.paise();

    const std::int64_t rupees =
        paise / 100;

    const std::int64_t remainingPaise =
        paise % 100;

    std::cout
        << label
        << "Rs. "
        << rupees
        << '.';

    if (remainingPaise < 10)
    {
        std::cout << '0';
    }

    std::cout
        << remainingPaise
        << '\n';
}

int handleGenerateTemplate(
    int argc,
    char* argv[]
)
{
    if (argc != 4)
    {
        printUsage();
        return 1;
    }

    const std::size_t debtCount =
        parseDebtCount(argv[2]);

    const std::filesystem::path outputPath{
        argv[3]
    };

    debtpilot::cli::DebtFileManager::
        generateTemplate(
            debtCount,
            outputPath
        );

    std::cout
        << "Template created: "
        << outputPath.string()
        << '\n';

    return 0;
}

int handleRun(int argc, char* argv[])
{
    if (argc != 3 && argc != 5)
    {
        printUsage();
        return 1;
    }

    const std::filesystem::path inputPath{
        argv[2]
    };

    const debtpilot::cli::DebtFileData fileData =
        debtpilot::cli::DebtFileManager::load(
            inputPath
        );

    const debtpilot::Money minimumBudget =
        debtpilot::MinimumBudgetCalculator::
            calculate(
                fileData.debts()
            );

    const debtpilot::Money monthlyBudget =
        fileData.monthlyBudget();

    std::cout
        << "DebtPilot repayment analysis\n"
        << "Input file: "
        << inputPath.string()
        << "\n\n";

    std::cout
        << "Debts loaded: "
        << fileData.debts().size()
        << '\n';

    printMoney(
        "Minimum required monthly budget: ",
        minimumBudget
    );

    printMoney(
        "Configured monthly budget:       ",
        monthlyBudget
    );

    if (monthlyBudget < minimumBudget)
    {
        const debtpilot::Money shortfall =
            minimumBudget - monthlyBudget;

        printMoney(
            "Monthly budget shortfall:      ",
            shortfall
        );

        std::cout
            << '\n'
            << "The configured monthly budget is below "
            << "the combined minimum payments.\n"
            << "A Snowball or Avalanche portfolio plan "
            << "cannot be generated with this budget.\n";

        return 2;
    }

    const debtpilot::Money strategyBudget =
        monthlyBudget - minimumBudget;

    printMoney(
        "Available strategic payment:     ",
        strategyBudget
    );

    std::cout << '\n';

    const debtpilot::DebtPlanningRequest request{
        fileData.debts(),
        monthlyBudget,
        fileData.maximumMonths()
    };

    const debtpilot::DebtPlanningService service;

    const auto comparison =
        service.compareStrategies(request);

    const debtpilot::cli::ConsoleReporter reporter;

    reporter.printComparison(comparison);

    if (argc == 5)
    {
        const std::string option{
            argv[3]
        };

        const std::string strategy{
            argv[4]
        };

        if (option != "--details")
        {
            throw std::invalid_argument{
                "Expected --details option"
            };
        }

        if (strategy == "snowball")
        {
            reporter.printRepaymentPlan(
                comparison.snowballPlan(),
                fileData.debts(),
                "SNOWBALL"
            );
        }
        else if (strategy == "avalanche")
        {
            reporter.printRepaymentPlan(
                comparison.avalanchePlan(),
                fileData.debts(),
                "AVALANCHE"
            );
        }
        else
        {
            throw std::invalid_argument{
                "Strategy must be snowball or avalanche"
            };
        }
    }

    return 0;
}
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            printUsage();
            return 1;
        }

        const std::string command{
            argv[1]
        };

        if (command == "generate-template")
        {
            return handleGenerateTemplate(
                argc,
                argv
            );
        }

        if (command == "run")
        {
            return handleRun(
                argc,
                argv
            );
        }

        throw std::invalid_argument{
            "Unknown command: " + command
        };
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "DebtPilot failed: "
            << exception.what()
            << '\n';

        return 1;
    }
}
