#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"

#include <vector>

namespace debtpilot
{

class MinimumBudgetCalculator
{
public:
    [[nodiscard]]
    static Money calculate(
        const std::vector<Debt>& debts
    );
};

}