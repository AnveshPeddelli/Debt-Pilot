#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class DebtPrioritizer
    {
        public:
            [[nodiscard]] static std::vector<std::size_t> prioritize(const std::vector<Debt>& debts, RepaymentStrategy strategy);
    };
}