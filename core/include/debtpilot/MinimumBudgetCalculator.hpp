#pragma once

#include "Money.hpp"
#include "Debt.hpp"

#include <vector>

namespace debtpilot
{
    class MinimumBudgetCalculator
    {
        public:
            [[nodiscard]] static Money Calculate(const std::vector<Debt>& debts);
    };
}