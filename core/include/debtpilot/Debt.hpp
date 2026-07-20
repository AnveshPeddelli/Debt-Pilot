#pragma once

#include "debtpilot/DebtType.hpp"
#include "debtpilot/InterestRate.hpp"
#include "debtpilot/Money.hpp"

#include <optional>
#include <string>

namespace debtpilot
{
    class Debt
    {
        public:
            Debt(
                std::string id,
                std::string name,
                DebtType type,
                Money outstandingBalance,
                InterestRate annualInterestRate,
                Money minimumPayment,
                std::optional<int> dueDay = std::nullopt);

            [[nodiscard]] const std::string& id() const noexcept;
            [[nodiscard]] const std::string& name() const noexcept;
            [[nodiscard]] DebtType type() const noexcept;
            [[nodiscard]] Money outstandingBalance() const noexcept;
            [[nodiscard]] InterestRate annualInterestRate() const noexcept;
            [[nodiscard]] Money minimumPayment() const noexcept;
            [[nodiscard]] std::optional<int> dueDay() const noexcept;

        private:
            std::string id_;
            std::string name_;
            DebtType type_;
            Money outstandingBalance_;
            InterestRate annualInterestRate_;
            Money minimumPayment_;
            std::optional<int> dueDay_;
    };
}
