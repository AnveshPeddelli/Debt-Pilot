#include "debtpilot/DeptMonthlySnapshot.hpp"

#include <utility>

namespace debtpilot
{
    DebtMonthlySnapshot::DebtMonthlySnapshot(std::string debtId, Money openingBalance, Money interestCharged, Money payment, Money principalRepaid, Money closingBalance) : debtId_{debtId}, openingBalance_{openingBalance}, interestCharged_{interestCharged}, payment_{payment}, principalRepaid_{principalRepaid}, closingBalance_{closingBalance}
    {

    }

    const std::string& DebtMonthlySnapshot::debtId() const noexcept
    {
        return debtId_;
    }

    Money DebtMonthlySnapshot::openingBalance() const noexcept
    {
        return openingBalance_;
    }

    Money DebtMonthlySnapshot::interestCharged() const noexcept
    {
        return interestCharged_;
    }

    Money DebtMonthlySnapshot::payment() const noexcept
    {
        return payment_;
    }

    Money DebtMonthlySnapshot::principalRepaid() const noexcept
    {
        return principalRepaid_;
    }

    Money DebtMonthlySnapshot::closingBalance() const noexcept
    {
        return closingBalance_;
    }

    
}