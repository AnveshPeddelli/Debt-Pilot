#include "debtpilot/PortfolioPaymentAllocation.hpp"

#include <utility>

namespace debtpilot
{
    PortfolioPaymentAllocation::PortfolioPaymentAllocation(std::vector<DebtPaymentAllocation> allocations, Money totalAllocated, Money unallocatedAmount) : allocations_{std::move(allocations)}, totalAllocated_{totalAllocated}, unallocatedAmount_{unallocatedAmount}
    {

    }

    const std::vector<DebtPaymentAllocation>& PortfolioPaymentAllocation::allocations() const noexcept
    {
        return allocations_;
    }

    Money PortfolioPaymentAllocation::totalAllocated() const noexcept
    {
        return totalAllocated_;
    }

    Money PortfolioPaymentAllocation::unallocatedAmount() const noexcept
    {
        return unallocatedAmount_;
    }
}