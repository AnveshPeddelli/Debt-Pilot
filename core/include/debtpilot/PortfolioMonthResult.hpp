#pragma once

#include "debtpilot/DeptMonthlySnapshot.hpp"
#include "debtpilot/Money.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class PortfolioMonthResult
    {
        public:
            PortfolioMonthResult(std::size_t monthNumber, std::vector<DebtMonthlySnapshot> debtSnapshot, Money totalInterest, Money totalPaid);

            [[nodiscard]] std::size_t monthNumber() const noexcept;
            [[nodiscard]] const std::vector<DebtMonthlySnapshot>& debtSnapshots() const noexcept;
            [[nodiscard]] Money totalInterest() const noexcept;
            [[nodiscard]] Money totalPaid() const noexcept;

        private:
            std::size_t monthNumber_;
            std::vector<DebtMonthlySnapshot> debtSnapshots_;
            Money totalInterest_;
            Money totalPaid_;
    };
}