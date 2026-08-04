#pragma once

#include "debtpilot/DebtMonthlySnapshot.hpp"
#include "debtpilot/Money.hpp"
#include "debtpilot/PlannedPaymentStatus.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class PlannedPaymentPayoffResult
    {
        public:
            PlannedPaymentPayoffResult(PlannedPaymentStatus status, Money requiredMonthlyPayment, Money plannedMonthlyPayment, std::vector<DebtMonthlySnapshot> monthlySnapshots, Money totalInterest, Money totalPaid);

            [[nodiscard]] PlannedPaymentStatus status() const noexcept;
            [[nodsicard]] Money requiredMonthlyPayment() const noexcept;
            [[nodiscard]] Money plannedMonthlyPayment() const noexcept;
            [[nodiscard]] const std::vector<DebtMonthlySnapshot>& monthlySnapshots() const noexcept;
            [[nodiscard]] std::size_t totalMonths() const noexcept;
            [[nodiscard]] Money totalInterest() const noexcept;
            [[nodiscard]] Money totalPaid() const noexcept;
            [[nodiscard]] bool isRepayable() const noexcept;
            [[nodiscard]] bool isBelowRequiredPayment() const noexcept;

        private:
            PlannedPaymentStatus status_;
            Money requiredMonthlyPayment_;
            Money plannedMonthlyPayment_;
            std::vector<DebtMonthlySnapshot> monthlySnapshots_;
            Money totalInterest_;
            Money totalPaid_;
    };
}