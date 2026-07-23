#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"
#include "debtpilot/PortfolioRepaymentPlan.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class PortfolioRepaymentSimulator
    {
        public:
            [[nodiscard]] static PortfolioRepaymentPlan simulate(std::vector<Debt> debts, Money monthlyBudget, RepaymentStrategy strategy, std::size_t maximumMonths = 1'200);
        
    };
}