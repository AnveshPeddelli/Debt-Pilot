#pragma once

#include "debtpilot/Money.hpp"
#include "debtpilot/PortfolioRepaymentPlan.hpp"
#include "debtpilot/RepaymentStrategy.hpp"

#include <cstddef>

namespace debtpilot
{
    class StrategyComparisonResult
    {
        public:
            StrategyComparisonResult(PortfolioRepaymentPlan snowballPlan, PortfolioRepaymentPlan avalanchePlan, RepaymentStrategy recommendedStrategy, Money interestSavings, std::size_t monthsSaved);

            [[nodiscard]] const PortfolioRepaymentPlan& snowballPlan() const noexcept;
            [[nodiscard]] const PortfolioRepaymentPlan& avalanchePlan() const noexcept;
            [[nodiscard]] RepaymentStrategy recommendedStrategy() const noexcept;
            [[nodiscard]] Money interestSavings() const noexcept;
            [[nodiscard]] std::size_t monthsSaved() const noexcept;

        private:
            PortfolioRepaymentPlan snowballPlan_;
            PortfolioRepaymentPlan avalanchePlan_;
            RepaymentStrategy recommendedStrategy_;
            Money interestSavings_;
            std::size_t monthsSaved_;
    };
}