#include "debtpilot/DebtPlanningRequest.hpp"
#include "debtpilot/DebtPlanningService.hpp"
#include "debtpilot/MinimumBudgetCalculator.hpp"
#include "debtpilot/cli/ConsoleReporter.hpp"
#include "debtpilot/cli/DebtFileManager.hpp"

#include <charconv>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{

std::size_t parseDebtCount(const std::string& text)
{
    std::size_t value{};

    const auto [position, error] =
        std::from_chars(
            text.data(),
            text.data() + text.size(),
            value
        );

    if (
        error != std::errc{} ||
        position != text.data() + text.size() ||
        value == 0
    ) {
        throw std::invalid_argument{
            "Debt count must be a positive whole number"
        };
    }

    return value;
}

void printUsage()
{
    std::cout
        << "Usage:\n"
        << "  debtpilot_cli generate-template "
        << "<debt-count> <output-file>\n"
        << "  debtpilot_cli run <input-file>\n";
}

}

int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            printUsage();
            return 1;
        }

        const std::string command = argv[1];

        if (command == "generate-template") {
            if (argc != 4) {
                printUsage();
                return 1;
            }

            const std::size_t debtCount =
                parseDebtCount(argv[2]);

            debtpilot::cli::DebtFileManager::generateTemplate(
                debtCount,
                std::filesystem::path{argv[3]}
            );

            std::cout
                << "Template created: "
                << argv[3]
                << '\n';

            return 0;
        }

        if (command == "run") {
            if (argc != 3) {
                printUsage();
                return 1;
            }

            const auto fileData =
                debtpilot::cli::DebtFileManager::load(
                    std::filesystem::path{argv[2]}
                );

            const debtpilot::Money minimumBudget =
                debtpilot::MinimumBudgetCalculator::calculate(
                    fileData.debts()
                );

            const debtpilot::Money finalBudget =
                minimumBudget +
                fileData.monthlyExtraPayment();

            std::cout
                << "Minimum required monthly budget: Rs. "
                << minimumBudget.paise() / 100
                << '\n';

            std::cout
                << "Configured extra payment: Rs. "
                << fileData.monthlyExtraPayment().paise() / 100
                << '\n';

            std::cout
                << "Final monthly budget: Rs. "
                << finalBudget.paise() / 100
                << '\n';

            const debtpilot::DebtPlanningRequest request{
                fileData.debts(),
                finalBudget,
                fileData.maximumMonths()
            };

            const debtpilot::DebtPlanningService service;
            const auto comparison =
                service.compareStrategies(request);

            const debtpilot::cli::ConsoleReporter reporter;

            reporter.printComparison(comparison);

            return 0;
        }

        throw std::invalid_argument{
            "Unknown command: " + command
        };
    }
    catch (const std::exception& exception) {
        std::cerr
            << "DebtPilot failed: "
            << exception.what()
            << '\n';

        return 1;
    }
}