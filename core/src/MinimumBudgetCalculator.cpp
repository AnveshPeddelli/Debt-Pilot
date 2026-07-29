#include "debtpilot/MinimumBudgetCalculator.hpp"
#include "debtpilot/MonthlyInterestCalculator.hpp"

namespace debtpilot
{

Money MinimumBudgetCalculator::calculate(
    const std::vector<Debt>& debts
)
{
    Money totalMinimum = Money::fromPaise(0);

    for (const Debt& debt : debts)
    {
        if (debt.outstandingBalance().isZero())
        {
            continue;
        }

        const Money interest =
            MonthlyInterestCalculator::calculate(
                debt.outstandingBalance(),
                debt.annualInterestRate()
            );

        const Money amountDue =
            debt.outstandingBalance() + interest;

        const Money requiredPayment =
            debt.minimumPayment() > amountDue
                ? amountDue
                : debt.minimumPayment();

        totalMinimum =
            totalMinimum + requiredPayment;
    }

    return totalMinimum;
}

}