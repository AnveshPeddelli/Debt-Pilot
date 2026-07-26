#include "debtpilot/DebtPlanningRequest.hpp"
#include "debtpilot/DebtPlanningService.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include "ConsoleInput.hpp"
#include "ConsoleReporter.hpp"

#include <exception>
#include <iostream>


int main()
{
    try
    {
        std::cout
        << "========================================\n"
        << "             DebtPilot CLI\n"
        << "========================================\n"
        << "Debt repayment planning with Snowball "
        << "and Avalanche strategies.\n\n";
        
        const debtpilot::cli::ConsoleInput input;
        const debtpilot::cli::ConsoleReporter reporter;

        const auto debts = input.readDebts();
        const debtpilot::Money monthlyBudget = input.readMonthlyBudget();
        const debtpilot::DebtPlanningRequest request{debts, monthlyBudget};
        const debtpilot::DebtPlanningService service;
        const auto comparison = service.compareStrategies(request);
        
        reporter.printPlan(comparison.snowballPlan(), debtpilot::RepaymentStrategy::Snowball);
        reporter.printPlan(comparison.avalanchePlan(), debtpilot::RepaymentStrategy::Avalanche);
        reporter.printComparison(comparison);

        return 0;
    }
    catch(const std::exception& exception)
    {
        std::cerr << "\nDebtPilot failed: "<< exception.what()<< '\n';
        return 1;
    }
}