#include "debtpilot/Money.hpp"

namespace debtpilot
{
    class MonthlyBudget
    {
        public:
            MonthlyBudget(Money income, Money essential, Money discretionaryExpenses, Money fixedCommitments, Money savingsContribution);

            [[nodiscard]] Money totalExpenses() const noexcept;
            [[nodiscard]] Money availableForDebt() const noexcept;

        private:
            Money income_;
            Money essentialExpenses_;
            Money discretionaryExpenses_;
            Money fixedCommitments_;
            Money savingsContribution_;
    };
}
