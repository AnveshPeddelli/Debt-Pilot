#include "debtpilot/DebtPlanningService.hpp"

#include "debtpilot/PortfolioRepaymentSimulator.hpp"
#include "debtpilot/StrategyComparisonEngine.hpp"

namespace debtpilot
{
    PortfolioRepaymentPlan DebtPlanningService::generatePlan(const DebtPlanningRequest& request, RepaymentStrategy strategy) const
    {
        return PortfolioRepaymentSimulator::simulate(request.debts(), request.monthlyBudget(), strategy, request.maximumMonths());
    }

    StrategyComparisonResult DebtPlanningService::compareStrategies(const DebtPlanningRequest& request) const
    {
        return StrategyComparisonEngine::compare(request.debts(), request.monthlyBudget(), request.maximumMonths());
    }

}