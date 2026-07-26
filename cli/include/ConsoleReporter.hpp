#pragma once

#include "debtpilot/PortfolioRepaymentPlan.hpp"
#include "debtpilot/RepaymentStrategy.hpp"
#include "debtpilot/StrategyComparisonResult.hpp"

#include <string>

namespace debtpilot::cli
{
    class ConsoleReporter
    {
        public:
            void printPlan(const PortfolioRepaymentPlan& plan, RepaymentStrategy strategy) const;
            void printComparison(const StrategyComparisonResult& result) const;

        private:
            [[nodiscard]] static std::string formatMoney(Money money);
            [[nodiscard]] static const char* strategyName(RepaymentStrategy strategy) noexcept;
    };
}