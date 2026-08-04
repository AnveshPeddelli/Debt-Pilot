#pragma once
 
#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"
#include "debtpilot/PlannedPaymentPayoffResult.hpp"

#include <cstddef>
#include <string>

namespace debtpilot
{
    class PlannedPaymentPayoffSimulator
    {
        public:
            [[nodiscard]] static PlannedPaymentPayoffResult simulate(std::string debtId, Money outstandingBalance, InterestRate annualInterestRate, Money requiredMonthlyPayment, Money plannedMonthlyPayment, std::size_t maximumMonths = 1'200);
    };
}