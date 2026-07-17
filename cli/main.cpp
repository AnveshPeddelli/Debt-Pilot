#include "debtpilot/Debt.hpp"

#include <iostream>

int main()
{
    using namespace debtpilot;

    const Debt debt{
        "7000",
        "icici card",
        DebtType::CreditCard,
        Money::fromPaise(45'000 * 100),
        InterestRate::fromBasisPoints(3600),
        Money::fromPaise(3'000 * 100),
        15
    };

    std::cout
        << "Debt: "<< debt.name()<< '\n'
        << "Balance in paise: "<< debt.outstandingBalance().paise() << '\n'
        << "APR basis points: "<< debt.annualInterestRate().basisPoints() << '\n';

    return 0;
}