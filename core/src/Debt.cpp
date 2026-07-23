#include "debtpilot/Debt.hpp"

#include <stdexcept>
#include <utility>

namespace debtpilot
{
    Debt::Debt(
        std::string id,
        std::string name,
        DebtType type,
        Money outstandingBalance,
        InterestRate annualInterestRate,
        Money minimumPayment,
        std::optional<int> dueDay)
        : id_{std::move(id)},
          name_{std::move(name)},
          type_{type},
          outstandingBalance_{outstandingBalance},
          annualInterestRate_{annualInterestRate},
          minimumPayment_{minimumPayment},
          dueDay_{dueDay}
    {
        if(id_.empty())
        {
            throw std::invalid_argument{"Debt ID cannot be empty"};
        }
    
        if(name_.empty())
        {
            throw std::invalid_argument{"Debt name cannot be empty"};
        }
    
        if(outstandingBalance_.paise() < 0)
        {
            throw std::invalid_argument{"Outstanding balance must be greater than zero"};
        }
    
        if(minimumPayment_.isNegative())
        {
            throw std::invalid_argument{"Minimum payment cannot be negative"};
        }
    
        if(dueDay_ && (*dueDay_ < 1 || *dueDay_ > 31))
        {
            throw std::invalid_argument{"Due day must be between 1 and 31"};
        }
    }

    const std::string& Debt::id() const noexcept
    {
        return id_;
    }

    const std::string& Debt::name() const noexcept
    {
        return name_;
    }

    DebtType Debt::type() const noexcept
    {
        return type_;
    }

    Money Debt::outstandingBalance() const noexcept
    {
        return outstandingBalance_;
    }

    InterestRate Debt::annualInterestRate() const noexcept
    {
        return annualInterestRate_;
    }

    Money Debt::minimumPayment() const noexcept
    {
        return minimumPayment_;
    }

    std::optional<int> Debt::dueDay() const noexcept
    {
        return dueDay_;
    }

}    
