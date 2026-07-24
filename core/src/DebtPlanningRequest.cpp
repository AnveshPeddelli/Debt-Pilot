#include "debtpilot/DebtPlanningRequest.hpp"

#include <stdexcept>
#include <utility>

namespace debtpilot
{
    DebtPlanningRequest::DebtPlanningRequest(std::vector<Debt> debts, Money monthlyBudget, std::size_t maximumMonths) : debts_{std::move(debts)}, monthlyBudget_{monthlyBudget}, maximumMonths_{maximumMonths}
    {
        if(monthlyBudget_.paise() < 0)
        {
            throw std::invalid_argument{"Monthly budget cannot be negative"};
        }
        if(maximumMonths_ == 0)
        {
            throw std::invalid_argument{"Maximum months must be greater than zero"};
        }
    }

    const std::vector<Debt>& DebtPlanningRequest::debts() const noexcept
    {
        return debts_;
    }

    Money DebtPlanningRequest::monthlyBudget() const noexcept
    {
        return monthlyBudget_;
    }

    std::size_t DebtPlanningRequest::maximumMonths() const noexcept
    {
        return maximumMonths_;
    }

}