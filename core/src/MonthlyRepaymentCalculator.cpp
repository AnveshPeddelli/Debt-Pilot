#include "debtpilot/MonthlyRepaymentCalculator.hpp"
#include "debtpilot/MonthlyInterestCalculator.hpp"
#include <stdexcept>

namespace debtpilot
{
    MonthlyRepaymentResult MonthlyRepaymentCalculator::calculate(Money openingBalance, InterestRate annualInterestRate, Money requestedPayment)
    {
        if(openingBalance.paise() < 0)
        {
            throw std::invalid_argument{"Opening balance cannot be negative"};
        }     
 
        if(requestedPayment.paise() < 0)
        {
            throw std::invalid_argument{"Requested payment cannot be negative"};
        }

        if(openingBalance.isZero())
        {
            return MonthlyRepaymentResult{openingBalance, Money::fromPaise(0), requestedPayment, Money::fromPaise(0), Money::fromPaise(0), Money::fromPaise(0)};
        }

        const Money interestCharged = MonthlyInterestCalculator::calculate(openingBalance, annualInterestRate);
        const Money amountDue = openingBalance + interestCharged;
        const Money actualPayment = requestedPayment > amountDue ? amountDue : requestedPayment;
        const Money closingBalance = amountDue - actualPayment;

        Money principalRepaid = Money::fromPaise(0);
        if(actualPayment > interestCharged)
        {
            principalRepaid = actualPayment - interestCharged;
        }
        
        return MonthlyRepaymentResult{openingBalance, interestCharged, requestedPayment, actualPayment, principalRepaid, closingBalance};
    }
}