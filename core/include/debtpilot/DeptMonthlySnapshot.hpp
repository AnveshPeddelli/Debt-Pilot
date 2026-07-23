#pragma once

#include "debtpilot/Money.hpp"

#include <string>

namespace debtpilot
{
    class DebtMonthlySnapshot
    {
        public:
            DebtMonthlySnapshot(std::string debtId, Money openingBalance, Money interestCharged, Money payment, Money principalRepaid, Money closingBalance);

            [[nodiscard]] const std::string& debtId() const noexcept;
            [[nodiscard]] Money openingBalance() const noexcept;
            [[nodiscard]] Money interestCharged() const noexcept;
            [[nodiscard]] Money payment() const noexcept;
            [[nodiscard]] Money principalRepaid() const noexcept;
            [[nodiscard]] Money closingBalance() const noexcept;

        private:
            std::string debtId_;
            Money openingBalance_;
            Money interestCharged_;
            Money payment_;
            Money principalRepaid_;
            Money closingBalance_;
            
    };
}