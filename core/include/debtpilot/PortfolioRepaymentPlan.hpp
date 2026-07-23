#pragma once

#include "debtpilot/Money.hpp"
#include "debtpilot/PortfolioMonthResult.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace debtpilot
{
    class PortfolioRepaymentPlan
    {
        public:
            PortfolioRepaymentPlan(std::vector<PortfolioMonthResult> monthlyResults, std::unordered_map<std::string, std::size_t> payoffMonths, Money totalInterest, Money totalPaid);

            [[nodiscard]] const std::vector<PortfolioMonthResult>& monthlyResults() const noexcept;
            [[nodiscard]] const std::unordered_map<std::string, std::size_t>& payoffMonths() const noexcept;
            [[nodiscard]] std::size_t totalMonths() const noexcept;
            [[nodiscard]] Money totalInterest() const noexcept;
            [[nodiscard]] Money totalPaid() const noexcept;

        private:
        std::vector<PortfolioMonthResult> monthlyResults_;
        std::unordered_map<std::string, std::size_t> payoffMonths_;
        Money totalInterest_;
        Money totalPaid_;
        
    };
}
