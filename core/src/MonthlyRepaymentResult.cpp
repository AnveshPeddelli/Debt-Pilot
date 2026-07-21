#include "debtpilot/MonthlyRepaymentResult.hpp"


namespace debtpilot
{
    MonthlyRepaymentResult::MonthlyRepaymentResult(Money openingBalance, 
        Money interestCharged, Money requestedPayment, Money actualPayment, 
        Money principalRepaid, Money closingBalance) noexcept : 
        openingBalance_{openingBalance}, interestCharged_{interestCharged}, 
        requestedPayment_{requestedPayment}, actualPayment_{actualPayment}, 
        principalRepaid_{principalRepaid}, closingBalance_{closingBalance}
    {

    }

    Money MonthlyRepaymentResult::openingBalance() const noexcept
    {
        return openingBalance_;
    }

    Money MonthlyRepaymentResult::interestCharged() const noexcept
    {
        return interestCharged_;
    }

    Money MonthlyRepaymentResult::requestedPayment() const noexcept
    {
        return requestedPayment_;
    }

    Money MonthlyRepaymentResult::actualPayment() const noexcept
    {
        return actualPayment_;
    }

    Money MonthlyRepaymentResult::principalRepaid() const noexcept
    {
        return principalRepaid_;
    }

    Money MonthlyRepaymentResult::closingBalance() const noexcept
    {
        return closingBalance_;
    }

    bool MonthlyRepaymentResult::isFullyRepaid() const noexcept
    {
        return closingBalance_.isZero();
    }
        
}