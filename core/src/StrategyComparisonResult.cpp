#include "debtpilot/StrategyComparisonResult.hpp"

#include <utility>

namespace debtpilot
{
    StrategyComparisonResult::StrategyComparisonResult(PortfolioRepaymentPlan snowballPlan, PortfolioRepaymentPlan avalanchePlan, RepaymentStrategy recommendedStrategy, Money interestSavings, std::size_t monthsSaved) : snowballPlan_{std::move(snowballPlan)}, avalanchePlan_{std::move(avalanchePlan)}, recommendedStrategy_{recommendedStrategy}, interestSavings_{interestSavings}, monthsSaved_{monthsSaved}
    {

    }

    const PortfolioRepaymentPlan& StrategyComparisonResult::snowballPlan() const noexcept
    {
        return snowballPlan_;
    }

    const PortfolioRepaymentPlan& StrategyComparisonResult::avalanchePlan() const noexcept
    {
        return avalanchePlan_;
    }

    RepaymentStrategy StrategyComparisonResult::recommendedStrategy() const noexcept
    {
        return recommendedStrategy_;
    }

    Money StrategyComparisonResult::interestSavings() const noexcept
    {
        return interestSavings_;
    }

    std::size_t StrategyComparisonResult::monthsSaved() const noexcept
    {
        return monthsSaved_;
    }
}