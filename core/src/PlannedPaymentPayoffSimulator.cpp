#include "debtpilot/PlannedPaymentPayoffSimulator.hpp"

#include "debtpilot/MonthlyInterestCalculator.hpp"
#include "debtpilot/MonthlyRepaymentCalculator.hpp"
#include "debtpilot/PlannedPaymentStatus.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

namespace debtpilot
{
    PlannedPaymentPayoffResult PlannedPaymentPayoffSimulator::simulate(std::string debtId, Money outstandingBalance, InterestRate annualInterestRate, Money requiredMonthlyPayment, Money plannedMonthlyPayment, std::size_t maximumMonths)
    {
        if(debtId.empty())
        {
            throw std::invalid_argument{"Debt ID cannot be empty"};
        }

        if(outstandingBalance.paise() < 0)
        {
            throw std::invalid_argument{"Outstanding balance cannot be negative"};
        }

        if(requiredMonthlyPayment.paise() < 0)
        {
            throw std::invalid_argument{"Required monthly payment cannot be negative"};
        }

        if(plannedMonthlyPayment.paise() < 0)
        {
           throw std::invalid_argument{"Planned monthly payment cannot be negative"};
        }

        if(maximumMonths == 0)
        {
            throw std::invalid_argument{"Maximum months must be greater than zero"};
        }

        if(outstandingBalance.isZero())
        {
            return PlannedPaymentPayoffResult{
                PlannedPaymentStatus::Repaid,
                requiredMonthlyPayment,
                plannedMonthlyPayment,
                {},
                Money::fromPaise(0),
                Money::fromPaise(0)
            };
        }

        const Money firstMonthInterest = MonthlyInterestCalculator::calculate(outstandingBalance, annualInterestRate);
        if(plannedMonthlyPayment <= firstMonthInterest)
        {
            return PlannedPaymentPayoffResult{
                PlannedPaymentStatus::PaymentDoesNotCoverInterest,
                requiredMonthlyPayment,
                plannedMonthlyPayment,
                {},
                Money::fromPaise(0),
                Money::fromPaise(0)
            };
        }

        PlannedPaymentStatus status = plannedMonthlyPayment < requiredMonthlyPayment ? PlannedPaymentStatus::BelowRequiredPayment : PlannedPaymentStatus::Repaid;

        std::vector<DebtMonthlySnapshot> snapshots;
        snapshots.reserve(maximumMonths);

        Money currentBalance = outstandingBalance;
        Money totalInterest = Money::fromPaise(0);
        Money totalPaid = Money::fromPaise(0);

        for(std::size_t month = 1; month <= maximumMonths; ++month)
        {
            const MonthlyRepaymentResult repayment = MonthlyRepaymentCalculator::calculate(currentBalance, annualInterestRate, plannedMonthlyPayment);

            if(repayment.closingBalance() >= currentBalance)
            {
                return PlannedPaymentPayoffResult{
                    PlannedPaymentStatus::PaymentDoesNotCoverInterest,
                    requiredMonthlyPayment,
                    plannedMonthlyPayment,
                    std::move(snapshots),
                    totalInterest,
                    totalPaid
                };
            }

            const Money actualPayment = repayment.actualPayment();
            const Money minimumComponent = actualPayment < requiredMonthlyPayment ? actualPayment : requiredMonthlyPayment;
            const Money extraComponent = actualPayment > requiredMonthlyPayment ? actualPayment - requiredMonthlyPayment : Money::fromPaise(0);

            snapshots.emplace_back(
                debtId,
                repayment.openingBalance(),
                repayment.interestCharged(),                minimumComponent,
                extraComponent,
                actualPayment,
                repayment.principalRepaid(),
                repayment.closingBalance(),
                true
            );


            totalInterest = totalInterest + repayment.interestCharged();
            totalPaid = totalPaid + repayment.actualPayment();
            currentBalance = repayment.closingBalance();

            if(currentBalance.isZero())
            {
                return PlannedPaymentPayoffResult{status, requiredMonthlyPayment, plannedMonthlyPayment, std::move(snapshots), totalInterest, totalPaid};
            }
        }

        throw std::runtime_error{"Debt was not repaid within the maximum month"};
    }


}