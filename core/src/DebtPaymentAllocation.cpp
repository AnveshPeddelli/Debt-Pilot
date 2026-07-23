#include "debtpilot/DebtPaymentAllocation.hpp"

#include <utility>

namespace debtpilot
{
    DebtPaymentAllocation::DebtPaymentAllocation(std::string debtId, Money minimumPayment, Money extraPayment, Money totalPayment) : debtId_{std::move(debtId)}, minimumPayment_{minimumPayment}, extraPayment_{extraPayment}, totalPayment_{totalPayment}
    {

    }

    const std::string& DebtPaymentAllocation::debtId() const noexcept
    {
        return debtId_;
    }

    Money DebtPaymentAllocation::minimumPayment() const noexcept
    {
        return minimumPayment_;
    }

    Money DebtPaymentAllocation::extraPayment() const noexcept
    {
        return extraPayment_;
    }

    Money DebtPaymentAllocation::totalPayment() const noexcept
    {
        return totalPayment_;
    }
}