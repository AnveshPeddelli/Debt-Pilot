#pragma once

#include "debtpilot/Money.hpp"

#include <string>

namespace debtpilot
{
    class DebtMonthlySnapshot
    {
        public:
            DebtMonthlySnapshot(std::string debtId, Money openingBalance, Money interestCharged, Money minimumPayment, Money extraPayment, Money payment, Money principalRepaid, Money closingBalance, bool priorityDebt);

            [[nodiscard]] const std::string& debtId() const noexcept;
            [[nodiscard]] Money openingBalance() const noexcept;
            [[nodiscard]] Money interestCharged() const noexcept;
            [[nodiscard]] Money minimumPayment() const noexcept;
            [[nodiscard]] Money extraPayment() const noexcept;
            [[nodiscard]] Money payment() const noexcept;
            [[nodiscard]] Money principalRepaid() const noexcept;
            [[nodiscard]] Money closingBalance() const noexcept;
            [[nodiscard]] bool isPriorityDebt() const noexcept;
            [[nodiscard]] bool isFinalPayment() const noexcept;
        

        private:
            std::string debtId_;
            Money openingBalance_;
            Money interestCharged_;
            Money minimumPayment_;
            Money extraPayment_;
            Money payment_;
            Money principalRepaid_;
            Money closingBalance_;
            bool priorityDebt_;
            
    };
}