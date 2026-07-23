#pragma once

#include "debtpilot/PortfolioPaymentAllocation.hpp"
#include "debtpilot/Debt.hpp";
#include "debtpilot/Money.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include <vector>

namespace debtpilot
{
    class PortfolioPaymentAllocator
    {
        public:
            static PortfolioPaymentAllocation allocate(const std::vector<Debt>& debts, Money availableBudget, RepaymentStrategy strategy);
    };
}