#pragma once

#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"
#include "debtpilot/RepaymentSchedule.hpp"

#include<cstddef>

namespace debtpilot
{
    class RepaymentScheduleGenerator
    {
        public:
            [[nodiscard]] static RepaymentSchedule generate(Money openingBalance, InterestRate annualInterestRate, Money monthlyPayment, std::size_t maximumMonths = 1'200);
    };
}