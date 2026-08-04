#include "debtpilot/DebtMonthlySnapshot.hpp"

#include <utility>

namespace debtpilot
{
    DebtMonthlySnapshot::DebtMonthlySnapshot(std::string debtId, Money openingBalance, Money interestCharged, Money minimumPayment, Money extraPayment, Money payment, Money principalRepaid, Money closingBalance, bool priorityDebt) : 
    debtId_{debtId}, openingBalance_{openingBalance}, interestCharged_{interestCharged}, minimumPayment_{minimumPayment}, extraPayment_{extraPayment}, payment_{payment}, principalRepaid_{principalRepaid}, closingBalance_{closingBalance}, priorityDebt_{priorityDebt}
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

    Money DebtMonthlySnapshot::minimumPayment() const noexcept
    {
        return minimumPayment_;
    }

    Money DebtMonthlySnapshot::extraPayment() const noexcept
    {
        return extraPayment_;
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

    bool DebtMonthlySnapshot::isPriorityDebt() const noexcept
    {
        return priorityDebt_;
    }

    bool DebtMonthlySnapshot::isFinalPayment() const noexcept
    {
        return closingBalance_.isZero();
    }
    
}