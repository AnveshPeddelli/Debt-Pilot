#include "debtpilot/MonthlyBudget.hpp"

namespace debtpilot
{
    MonthlyBudget::MonthlyBudget(Money income, Money essentialExpenses, Money discretionaryExpenses, Money fixedCommitments, Money savingsContribution):
        income_{income}, essentialExpenses_{essentialExpenses}, discretionaryExpenses_{discretionaryExpenses}, fixedCommitments_{fixedCommitments}, savingsContribution_{savingsContribution}
    {

    }

    Money MonthlyBudget::totalExpenses() const noexcept
    {
        return essentialExpenses_ + discretionaryExpenses_ + fixedCommitments_ + savingsContribution_;
    }

    Money MonthlyBudget::availableForDebt() const noexcept
    {
        return income_ - totalExpenses();
    }
}