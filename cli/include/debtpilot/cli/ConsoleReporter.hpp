#pragma once

#include "debtpilot/Debt.hpp"

#include "debtpilot/PortfolioRepaymentPlan.hpp"
#include "debtpilot/RepaymentStrategy.hpp"
#include "debtpilot/StrategyComparisonResult.hpp"

#include "debtpilot/DebtMonthlySnapshot.hpp"
#include "debtpilot/PortfolioMonthResult.hpp"

#include <string>
#include <unordered_map>
#include <vector>


namespace debtpilot::cli
{
    class ConsoleReporter
    {
        public:
            void printPlan(const PortfolioRepaymentPlan& plan, RepaymentStrategy strategy) const;
            void printComparison(const StrategyComparisonResult& result) const;
            void printRepaymentPlan(const PortfolioRepaymentPlan& plan, const std::vector<Debt>& debts, const std::string& strategyName) const;

        private:
            [[nodiscard]] static std::string formatMoney(Money money);
            [[nodiscard]] static const char* strategyName(RepaymentStrategy strategy) noexcept;
    };
}