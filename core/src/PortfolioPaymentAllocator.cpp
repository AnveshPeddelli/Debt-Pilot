#include "debtpilot/PortfolioPaymentAllocator.hpp"

#include "debtpilot/DebtPrioritizer.hpp"
#include "debtpilot/MonthlyInterestCalculator.hpp"

#include <stdexcept>
#include <vector>

namespace debtpilot
{
    PortfolioPaymentAllocation PortfolioPaymentAllocator::allocate(const std::vector<Debt>& debts, Money availableBudget, RepaymentStrategy strategy)
    {
        if(availableBudget.paise() < 0)
        {
            throw std::invalid_argument{"Available budget cannot be negative"};
        }

        if(debts.empty())
        {
            return PortfolioPaymentAllocation{ {}, Money::fromPaise(0), availableBudget };
        }

        std::vector<Money> minimumAllocations(debts.size(), Money::fromPaise(0));

        Money totalMinimumRequired = Money::fromPaise(0);

        for(std::size_t index = 0; index < debts.size(); ++index)
        {
            const Debt& debt = debts[index];
            if(debt.outstandingBalance().isZero())
            {
                continue;
            }
            const Money interest = MonthlyInterestCalculator::calculate(debt.outstandingBalance(), debt.annualInterestRate());
            const Money amountDue = debt.outstandingBalance() + interest;
            const Money minimumPayment = debt.minimumPayment() > amountDue ? amountDue : debt.minimumPayment();
            
            minimumAllocations[index] = minimumPayment;
            totalMinimumRequired = totalMinimumRequired + minimumPayment;
        }

        if(availableBudget < totalMinimumRequired)
        {
            throw std::invalid_argument{"Available budget is below total minimum payments"};
        }

        std::vector<Money> extraAllocations(debts.size(), Money::fromPaise(0));
        Money remainingBudget = availableBudget - totalMinimumRequired;

        const std::vector<std::size_t> priorityOrder = DebtPrioritizer::prioritize(debts, strategy);

        for(const std::size_t index : priorityOrder)
        {
            if(remainingBudget.isZero())
            {
                break;
            }
            const Debt& debt = debts[index];

            if(debt.outstandingBalance().isZero())
            {
                continue;
            }
            
            const Money interest = MonthlyInterestCalculator::calculate(debt.outstandingBalance(), debt.annualInterestRate());
            const Money amountDue = debt.outstandingBalance() + interest;
            const Money remainingAmountDue = amountDue - minimumAllocations[index];

            if(remainingAmountDue.paise() <= 0)
            {
                continue;
            }

            const Money extraPayment = remainingBudget > remainingAmountDue ? remainingAmountDue : remainingBudget;
            extraAllocations[index] = extraPayment;
            remainingBudget = remainingBudget - extraPayment;
        }

        std::vector<DebtPaymentAllocation> allocations;
        allocations.reserve(debts.size());

        Money totalAllocated = Money::fromPaise(0);

        for(std::size_t index = 0; index < debts.size(); ++index)
        {
            const Money totalPayment = minimumAllocations[index] + extraAllocations[index];
            allocations.emplace_back(debts[index].id(), minimumAllocations[index], extraAllocations[index], totalPayment);
            totalAllocated = totalAllocated + totalPayment;
        }

        return PortfolioPaymentAllocation{std::move(allocations), totalAllocated, remainingBudget};
    }
}