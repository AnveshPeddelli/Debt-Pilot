#pragma once

#include "debtpilot/Money.hpp"
#include "debtpilot/MonthlyRepaymentResult.hpp"

#include <cstddef>
#include <vector>


namespace debtpilot
{
    class RepaymentSchedule
    {
        public:
            RepaymentSchedule(std::vector<MonthlyRepaymentResult> monthlyResults, Money totalInterest, Money totalPaid);

            [[nodiscard]] const std::vector<MonthlyRepaymentResult>& monthlyResults() const noexcept;
            [[nodiscard]] std::size_t payoffMonths() const noexcept;
            [[nodiscard]] Money totalInterest() const noexcept;
            [[nodiscard]] Money totalPaid() const noexcept;

        private:
            std::vector<MonthlyRepaymentResult> monthlyResults_;
            Money totalInterest_;
            Money totalPaid_;
    };
}