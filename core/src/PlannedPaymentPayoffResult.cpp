#include "debtpilot/PlannedPaymentPayoffResult.hpp"

#include <utility>

namespace debtpilot
{
    PlannedPaymentPayoffResult::PlannedPaymentPayoffResult(PlannedPaymentStatus status, Money requiredMonthlyPayment, Money plannedMonthlyPayment, std::vector<DebtMonthlySnapshot> monthlySnapshots, Money totalInterest, Money totalPaid) : 
    status_{status}, requiredMonthlyPayment_{plannedMonthlyPayment}, plannedMonthlyPayment_{plannedMonthlyPayment}, monthlySnapshots_{std::move(monthlySnapshots)}, totalInterest_{totalInterest}, totalPaid_{totalPaid}
    {

    }

    PlannedPaymentStatus PlannedPaymentPayoffResult::status() const noexcept
    {
        return status_;
    }

    Money PlannedPaymentPayoffResult::requiredMonthlyPayment() const noexcept
    {
        return requiredMonthlyPayment_;
    }

    Money PlannedPaymentPayoffResult::plannedMonthlyPayment() const noexcept
    {
        return plannedMonthlyPayment_;
    }

    const std::vector<DebtMonthlySnapshot>& PlannedPaymentPayoffResult::monthlySnapshots() const noexcept
    {
        return monthlySnapshots_;
    }

    Money PlannedPaymentPayoffResult::totalInterest() const noexcept
    {
        return totalInterest_;
    }

    Money PlannedPaymentPayoffResult::totalPaid() const noexcept
    {
        return totalPaid_;
    }

    bool PlannedPaymentPayoffResult::isRepayable() const noexcept
    {
        return status_ != PlannedPaymentStatus::PaymentDoesNotCoverInterest;
    }

    bool PlannedPaymentPayoffResult::isBelowRequiredPayment() const noexcept
    {
        return plannedMonthlyPayment_ < requiredMonthlyPayment_;
    }
}