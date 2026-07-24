#include "debtpilot/StrategyComparisonEngine.hpp"

#include "debtpilot/PortfolioRepaymentSimulator.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include <utility>

namespace debtpilot
{
    StrategyComparisonResult StrategyComparisonEngine::compare(const std::vector<Debt>& debts, Money monthlyBudget, std::size_t maximumMonths)
    {
        PortfolioRepaymentPlan snowballPlan = PortfolioRepaymentSimulator::simulate(debts, monthlyBudget, RepaymentStrategy::Snowball, maximumMonths);
        PortfolioRepaymentPlan avalanchePlan = PortfolioRepaymentSimulator::simulate(debts, monthlyBudget, RepaymentStrategy::Avalanche, maximumMonths);

        RepaymentStrategy recommededStrategy = RepaymentStrategy::Avalanche;

        if(snowballPlan.totalInterest() < avalanchePlan.totalInterest())
        {
            recommededStrategy = RepaymentStrategy::Snowball;
        }
        else if(snowballPlan.totalInterest() == avalanchePlan.totalInterest() && snowballPlan.totalMonths() < avalanchePlan.totalMonths())
        {
            recommededStrategy = RepaymentStrategy::Snowball;
        }

        const Money interetSavings = snowballPlan.totalInterest()>avalanchePlan.totalInterest() ? snowballPlan.totalInterest()-avalanchePlan.totalInterest() : avalanchePlan.totalInterest()-snowballPlan.totalInterest();
        const std::size_t monthsSaved = snowballPlan.totalMonths()>avalanchePlan.totalMonths() ? snowballPlan.totalMonths()-avalanchePlan.totalMonths() : avalanchePlan.totalMonths()-snowballPlan.totalMonths();

        return StrategyComparisonResult{std::move(snowballPlan), std::move(avalanchePlan), recommededStrategy, interetSavings, monthsSaved};
    }
}