#include "debtpilot/PortfolioRepaymentSimulator.hpp"

#include "debtpilot/MonthlyRepaymentCalculator.hpp"
#include "debtpilot/PortfolioPaymentAllocator.hpp"

#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>

namespace debtpilot
{
    PortfolioRepaymentPlan PortfolioRepaymentSimulator::simulate(std::vector<Debt> debts, Money monthlyBudget, RepaymentStrategy strategy, std::size_t maximumMonths)
    {
        if(monthlyBudget.paise() < 0)
        {
            throw std::invalid_argument{"Monthly budget cannot be negative"};      
        }

        if(maximumMonths == 0)
        {
            throw std::invalid_argument{"Maximum months must be greater than zero"};
        }

        std::unordered_set<std::string> debtIds;

        for(const Debt& debt : debts)
        {
            if(!debtIds.insert(debt.id()).second)
            {
                throw std::invalid_argument{"Debt IDs must be unique"};
            }
        }

        std::vector<PortfolioMonthResult> monthlyResults;
        std::unordered_map<std::string, std::size_t> payoffMonths;
        Money totalInterest = Money::fromPaise(0);
        Money totalPaid = Money::fromPaise(0);

        const auto allDebtsRepaid = [&debts]()
        {
            for(const Debt& debt : debts)
            {
                if(!debt.outstandingBalance().isZero())
                {
                    return false;
                }
            }
            return true;
        };


        while(!allDebtsRepaid())
        {
            if(monthlyResults.size() >= maximumMonths)
            {
                throw std::invalid_argument{"Portfolio was not repaid within the maximum month limit"};
            }

            const std::size_t monthNumber = monthlyResults.size() + 1;
            const PortfolioPaymentAllocation allocationResult = PortfolioPaymentAllocator::allocate(debts, monthlyBudget, strategy);
            if(allocationResult.totalAllocated().isZero())
            {
                throw std::runtime_error{"Monthly budget does not reduce the portfolio"};
            }

            const auto& allocations = allocationResult.allocations();
            if(allocations.size() != debts.size())
            {
                throw std::logic_error{"Payment allocation count does not match debt count"};
            }

            std::vector<DebtMonthlySnapshot> debtSnapshots;
            debtSnapshots.reserve(allocations.size());

            Money monthInterest = Money::fromPaise(0);
            Money monthPaid = Money::fromPaise(0);
            bool portfolioMadeProgress = false;

            for(std::size_t index = 0; index < debts.size(); ++index)
            {
                Debt& debt = debts[index];
                const Money openingBalance = debt.outstandingBalance();

                if(openingBalance.isZero())
                {
                    
                    debtSnapshots.emplace_back(
                        debt.id(), 
                        openingBalance, 
                        Money::fromPaise(0),    
                        Money::fromPaise(0), 
                        Money::fromPaise(0), 
                        Money::fromPaise(0), 
                        Money::fromPaise(0), 
                        Money::fromPaise(0), 
                        false);
                    continue;
                }
                
                const Money payment = allocations[index].totalPayment();
                const MonthlyRepaymentResult repayment = MonthlyRepaymentCalculator::calculate(openingBalance, debt.annualInterestRate(), payment);
                if(repayment.closingBalance() < openingBalance)
                {
                    portfolioMadeProgress = true;
                }

                debt.updateOutstandingBalance(repayment.closingBalance());

                monthInterest = monthInterest + repayment.interestCharged();
                monthPaid = monthPaid + repayment.actualPayment();
                debtSnapshots.emplace_back(debt.id(), repayment.openingBalance(), repayment.interestCharged(), allocations[index].minimumPayment(), allocations[index].extraPayment(), repayment.actualPayment(), repayment.principalRepaid(), repayment.closingBalance(), allocations[index].isPriorityDebt());

                if(repayment.isFullyRepaid() && !payoffMonths.contains(debt.id()))
                {
                    payoffMonths.emplace(debt.id(), monthNumber);
                }
            }
            
            if(!portfolioMadeProgress && !allDebtsRepaid())
            {
                throw std::runtime_error{"Monthly budget is too small to reduce the portfolio"};
            }

            totalInterest = totalInterest + monthInterest;
            totalPaid = totalPaid + monthPaid;
            monthlyResults.emplace_back(monthNumber, std::move(debtSnapshots), monthInterest, monthPaid);
        }

        return PortfolioRepaymentPlan{std::move(monthlyResults), std::move(payoffMonths), totalInterest, totalPaid};
    }

}