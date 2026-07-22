#include "debtpilot/DebtPrioritizer.hpp"

#include <algorithm>
#include <numeric>


namespace debtpilot
{
    std::vector<std::size_t> DebtPrioritizer::prioritize(const std::vector<Debt>& debts, RepaymentStrategy strategy)
    {
        std::vector<std::size_t> indices(debts.size());

        std::iota(indices.begin(), indices.end(), std::size_t(0));

        const auto snowballComparator = [&debts](std::size_t left, std::size_t right)
        {
            const Debt& first = debts[left];
            const Debt& second = debts[right];

            if(first.outstandingBalance() != second.outstandingBalance())
            {
                return first.outstandingBalance() < second.outstandingBalance();
            }

            if(first.annualInterestRate() != second.annualInterestRate())
            {
                return first.annualInterestRate() > second.annualInterestRate();
            }

            return first.id() < second.id();
        };

        const auto avalancheComparator = [&debts](std::size_t left, std::size_t right)
        {
            const Debt& first = debts[left];
            const Debt& second = debts[right];

            if(first.annualInterestRate() != second.annualInterestRate())
            {
                return first.annualInterestRate() > second.annualInterestRate();
            }

            if(first.outstandingBalance() != second.outstandingBalance())
            {
                return first.outstandingBalance() < second.outstandingBalance();
            }

            return first.id() < second.id();
        };

        switch(strategy)
        {
            case RepaymentStrategy::Snowball:
                std::sort(indices.begin(), indices.end(), snowballComparator);
                break;
            
            case RepaymentStrategy::Avalanche:
                std::sort(indices.begin(), indices.end(), avalancheComparator);
                break;
        }

        return indices;
    }
}