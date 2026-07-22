#include "debtpilot/RepaymentSchedule.hpp"

#include <utility>


namespace debtpilot
{
    RepaymentSchedule::RepaymentSchedule(std::vector<MonthlyRepaymentResult> monthlyResults, Money totalInterest, Money totalPaid) : monthlyResults_{std::move(monthlyResults)}, totalInterest_{totalInterest}, totalPaid_{totalPaid}
    {

    }

    const std::vector<MonthlyRepaymentResult>& RepaymentSchedule::monthlyResults() const noexcept
    {
        return monthlyResults_;
    }

    std::size_t RepaymentSchedule::payoffMonths() const noexcept
    {
        return monthlyResults_.size();
    }

    Money RepaymentSchedule::totalInterest() const noexcept
    {
        return totalInterest_;
    }

    Money RepaymentSchedule::totalPaid() const noexcept
    {
        return totalPaid_;
    }

}