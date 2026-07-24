#pragma once

#include "debtpilot/DebtPlanningRequest.hpp"
#include "debtpilot/PortfolioRepaymentPlan.hpp"
#include "debtpilot/RepaymentStrategy.hpp"
#include "debtpilot/StrategyComparisonResult.hpp"

namespace debtpilot
{
    class DebtPlanningService
    {
        public:
            [[nodiscard]] PortfolioRepaymentPlan generatePlan(const DebtPlanningRequest& request, RepaymentStrategy strategy) const;
            [[nodiscard]] StrategyComparisonResult compareStrategies(const DebtPlanningRequest& request) const;

    };
}