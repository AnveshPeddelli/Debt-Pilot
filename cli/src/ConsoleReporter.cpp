#include "debtpilot/cli/ConsoleReporter.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace debtpilot::cli
{
    void ConsoleReporter::printPlan(const PortfolioRepaymentPlan& plan, RepaymentStrategy strategy) const
    {
        std::cout
        << "\n===============================================\n"
        << strategyName(strategy)
        << " REPAYMENT PLAN\n"
        << "\n===============================================\n";
        
        std::cout
        << "Total payoff duration : "
        << plan.totalMonths()
        << " months\n";

        std::cout
        << "Total amount paid : "
        << formatMoney(plan.totalPaid())
        << '\n';

        std::cout<< "\nDebt payoff months\n";
        std::cout<< "-----------------------------------------------\n";

        std::vector<std::pair<std::string, std::size_t>> payofEntries{plan.payoffMonths().begin(), plan.payoffMonths().end()};

        for(const auto&[debtId, payoffMonth] : payofEntries)
        {
            std::cout
            << std::left
            << std::setw(20)
            << debtId
            << "Month "
            << payoffMonth
            << '\n';
        }

        std::cout << "\n===============================================\n";
    }

    void ConsoleReporter::printComparison(const StrategyComparisonResult& result) const
    {
        const auto& snowball = result.snowballPlan();
        const auto& avalanche = result.avalanchePlan();

        std::cout 
        << "\n===============================================\n"
        << "STRATEGY COMPARISION\n"
        << "\n===============================================\n";

        std::cout
        << std::left
        << std::setw(24)
        << "Metric"
        << std::setw(18)
        << "Snowball"
        << "Avalanche\n";

        std::cout<< "-----------------------------------------------"
        << "---------------------------\n";

        std::cout
        << std::left
        << std::setw(24)
        << "Payoff months"
        << std::setw(18)
        << snowball.totalMonths()
        << avalanche.totalMonths()
        << "\n";

        std::cout
        << std::left
        << std::setw(24)
        << "Total interest"
        << std::setw(18)
        << formatMoney(snowball.totalInterest())
        << formatMoney(avalanche.totalInterest())
        << '\n';

        std::cout
        << "\nRecommended strategy : "
        << strategyName(result.recommendedStrategy())
        << '\n';

        std::cout
        << "Interest difference : "
        << formatMoney(result.interestSavings())
        << '\n';

        std::cout
        << "Duration difference : "
        << result.monthsSaved()
        << " months\n";

        std::cout << "\n===============================================\n";
    }

    std::string ConsoleReporter::formatMoney(Money money)
    {
        const std::int64_t value = money.paise();
        const bool negative = value < 0;
        const std::uint64_t absolutevalue = negative ? static_cast<std::uint64_t>(-(value+1))+1 : static_cast<std::uint64_t>(value);
        const std::uint64_t rupees = absolutevalue / 100;
        const std::uint64_t paise = absolutevalue % 100;
        std::ostringstream output;

        if(negative)
        {
            output << '-';
        }

        output << "Rs. "<< rupees<< '.'<< std::setw(2)<< std::setfill('0')<< paise;

        return output.str();
    }

    const char* ConsoleReporter::strategyName(RepaymentStrategy strategy) noexcept
    {
        switch(strategy)
        {
            case RepaymentStrategy::Snowball:
                return "Snowball";
                
            case RepaymentStrategy::Avalanche:
                return "Avalanche";
        }

        return "unknown";
    }
    
}