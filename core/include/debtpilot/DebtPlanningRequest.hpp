#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"

#include <cstddef>
#include <vector>

namespace debtpilot
{
    class DebtPlanningRequest
    {
        public:
            DebtPlanningRequest(std::vector<Debt> debts, Money monthlyBudget, std::size_t maximumMonths = 1'200);

            [[nodiscard]] const std::vector<Debt>& debts() const noexcept;
            [[nodiscard]] Money monthlyBudget() const noexcept;
            [[nodiscard]] std::size_t maximumMonths() const noexcept;

        private:
            std::vector<Debt> debts_;
            Money monthlyBudget_;
            std::size_t maximumMonths_;
    };
}