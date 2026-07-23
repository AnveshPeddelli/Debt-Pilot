#pragma once

#include "debtpilot/DebtPaymentAllocation.hpp"
#include "debtpilot/Money.hpp"

#include <vector>

namespace debtpilot
{
    class PortfolioPaymentAllocation
    {
        public:
            PortfolioPaymentAllocation(std::vector<DebtPaymentAllocation> allocations, Money totalAllocated, Money unallocated);

            [[nodiscard]] const std::vector<DebtPaymentAllocation>& allocations() const noexcept;
            [[nodiscard]] Money totalAllocated() const noexcept;
            [[nodiscard]] Money unallocatedAmount() const noexcept;

        private:
            std::vector<DebtPaymentAllocation> allocations_;
            Money totalAllocated_;
            Money unallocatedAmount_;
    };
}