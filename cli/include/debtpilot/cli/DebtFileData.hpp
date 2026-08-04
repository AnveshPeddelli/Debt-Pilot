#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace debtpilot::cli
{

    class DebtFileData
    {
        public:
            DebtFileData(std::vector<Debt> debts, Money monthlyBudget, std::size_t maximumMonths)
                : debts_{std::move(debts)}, monthlyBudget_{monthlyBudget}, maximumMonths_{maximumMonths}
            {
            }

            [[nodiscard]] const std::vector<Debt>& debts() const noexcept
            {
                return debts_;
            }

            [[nodiscard]] Money monthlyBudget() const noexcept
            {
                return monthlyBudget_;
            }

            [[nodiscard]] std::size_t maximumMonths() const noexcept
            {
                return maximumMonths_;
            }

        private:
            std::vector<Debt> debts_;
            Money monthlyBudget_;
            std::size_t maximumMonths_;
    };

} // namespace debtpilot::cli