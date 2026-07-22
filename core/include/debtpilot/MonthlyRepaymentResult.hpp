#pragma once

#include "debtpilot/Money.hpp"

namespace debtpilot
{
    class MonthlyRepaymentResult
    {
        public:
            MonthlyRepaymentResult(Money openingBalance, Money interestCharged,
                 Money requestedPayment, Money actualPayment, Money principalRepaid, 
                 Money closingBalance) noexcept;

            [[nodiscard]] Money openingBalance() const noexcept;
            [[nodiscard]] Money interestCharged() const noexcept;
            [[nodiscard]] Money requestedPayment() const noexcept;
            [[nodiscard]] Money actualPayment() const noexcept;
            [[nodiscard]] Money principalRepaid() const noexcept;
            [[nodiscard]] Money closingBalance() const noexcept;
            
            [[nodiscard]] bool isFullyRepaid() const noexcept;
            
        private:
                Money openingBalance_;
                Money interestCharged_;
                Money requestedPayment_;
                Money actualPayment_;
                Money principalRepaid_;
                Money closingBalance_;
            
    };
}
