#include "debtpilot/MonthlyInterestCalculator.hpp"

#include <cstdint>
#include <limits>
#include <stdexcept>


namespace debtpilot
{
    Money MonthlyInterestCalculator::calculate(Money outstandingBalance, InterestRate annualInterestRate)
    {
        constexpr std::int64_t monthlyRateDivisor = 120'000;
        const std::int64_t balancePaise = outstandingBalance.paise();
        const std::int64_t annualBasisPoints = annualInterestRate.basisPoints();

        if(balancePaise < 0)
        {
            throw std::invalid_argument{"Outstanding balance cannot be negative"};
        }
        if(balancePaise == 0 || annualBasisPoints == 0)
        {
            return Money::fromPaise(0);
        }
        if(balancePaise > std::numeric_limits<std::int64_t>::max()/annualBasisPoints)
        {
            throw std::overflow_error{"Monthly interest calculation overflow"};
        }

        const std::int64_t numerator = balancePaise * annualBasisPoints;
        std::int64_t interestPaise = numerator/monthlyRateDivisor;
        const std::int64_t remainder = numerator % monthlyRateDivisor;

        //Round half a paise upward to the nearest whole paise
        if(remainder >= monthlyRateDivisor/2)
        {
            ++interestPaise;
        }

        return Money::fromPaise(interestPaise);
    }
}