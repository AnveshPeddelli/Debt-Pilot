#pragma once

#include "debtpilot/Debt.hpp"
#include "debtpilot/Money.hpp"

#include <cstddef>
#include <vector>
#include <utility>

namespace debtpilot::cli
{
    class DebtFileData
    {
        public:
            DebtFileData(std::vector<Debt> debts, Money monthlyExtraPayment, std::size_t maximumMonths) : 
            debts_{std::move(debts)}, monthlyExtraPayment_{monthlyExtraPayment}, maximumMonths_{maximumMonths}
            {

            }

            [[nodiscard]] const std::vector<Debt>& debts() const noexcept
            {
                return debts_;
            }

            [[nodiscard]] Money monthlyExtraPayment() const noexcept
            {
                return monthlyExtraPayment_;
            }

            [[nodiscard]] size_t maximumMonths() const noexcept
            {
                return maximumMonths_;
            }


        private:
            std::vector<Debt> debts_;
            Money monthlyExtraPayment_;
            std::size_t maximumMonths_;
    };
}