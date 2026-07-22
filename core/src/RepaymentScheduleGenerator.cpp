#include "debtpilot/RepaymentScheduleGenerator.hpp"

#include "debtpilot/MonthlyRepaymentCalculator.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

namespace debtpilot
{
    RepaymentSchedule RepaymentScheduleGenerator::generate(Money openingBalance, InterestRate annualInterestRate, Money monthlyPayment, std::size_t maximumMonths)
    {
        if(openingBalance.paise() < 0)
        {
            throw std::invalid_argument{"Opening balance cannot be negative"};
        }

        if(monthlyPayment.paise() <= 0 && !openingBalance.isZero())
        {
            throw std::invalid_argument{"Monthly payment must be greater than zero"};
        }

        if(maximumMonths == 0)
        {
            throw std::invalid_argument{"Maximum months must be greater than zero"};
        }

        std::vector<MonthlyRepaymentResult> monthlyResults;
        monthlyResults.reserve(maximumMonths);

        Money currentBalance = openingBalance;
        Money totalInterest = Money::fromPaise(0);
        Money totalPaid = Money::fromPaise(0);

        while(!currentBalance.isZero())
        {
            if(monthlyResults.size() >= maximumMonths)
            {
                throw std::invalid_argument{"Debt was not repaid within the maximum month limit"};
            }

            const MonthlyRepaymentResult result = MonthlyRepaymentCalculator::calculate(currentBalance, annualInterestRate, monthlyPayment);

            if(result.closingBalance() >= currentBalance && !result.closingBalance().isZero())
            {
                throw std::runtime_error{"Monthly payment is too small to reduce the debt"};
            }

            totalInterest = totalInterest + result.interestCharged();
            totalPaid = totalPaid + result.actualPayment();
            currentBalance = result.closingBalance();
            monthlyResults.push_back(result);
        }

        return RepaymentSchedule{std::move(monthlyResults), totalInterest, totalPaid};
    }

}