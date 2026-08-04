#pragma once

#include "debtpilot/Money.hpp"

#include <string>

namespace debtpilot
{
    class DebtPaymentAllocation
    {
        public:
            DebtPaymentAllocation(std::string debtId, Money minimumPayment, Money extraPayment, Money totalPayment, bool priorityDebt);

            [[nodiscard]] const std::string& debtId() const noexcept;
            [[nodiscard]] Money minimumPayment() const noexcept;
            [[nodiscard]] Money extraPayment() const noexcept;
            [[nodiscard]] Money totalPayment() const noexcept;
            [[nodiscard]] bool isPriorityDebt() const noexcept;

        private:
            std::string debtId_;
            Money minimumPayment_;
            Money extraPayment_;
            Money totalPayment_;
            bool priorityDebt_;
    };
}