#pragma once

#include "debtpilot/Money.hpp"
#include "debtpilot/InterestRate.hpp"
#include "debtpilot/MonthlyRepaymentResult.hpp"

namespace debtpilot
{
    class MonthlyRepaymentCalculator
    {
        public:
            [[nodiscard]] static MonthlyRepaymentResult calculate(Money openingBalance, InterestRate annualInterestRate, Money requestedPayment);

    };    
}