#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"
#include "debtpilot/StrategyComparisonResult.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class StrategyComparisonEngine
    {
        public:
            [[nodiscard]] static StrategyComparisonResult compare(const std::vector<Debt>& debts, Money monthlyBudget, std::size_t maximumMonths = 1'200);
    };
}