#pragma once

#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"


namespace debtpilot
{
    class MonthlyInterestCalculator
    {
        public:
            [[nodiscard]] static Money calculate(Money outstandingBalance, InterestRate annualInterestRate);
    };       
}   
