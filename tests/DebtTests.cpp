#include "debtpilot/Debt.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <stdexcept>

namespace
{
    using debtpilot::Debt;
    using debtpilot::DebtType;
    using debtpilot::InterestRate;
    using debtpilot::Money;

    Debt makeValidDebt(std::optional<int> dueDay = 15)
    {
        return Debt{
            "debt-1",
            "Credit card",
            DebtType::CreditCard,
            Money::fromPaise(250'000),
            InterestRate::fromBasisPoints(1'850),
            Money::fromPaise(10'000),
            dueDay};
    }
}

TEST(DebtTests, ConstructsValidDebt)
{
    const Debt debt = makeValidDebt();

    EXPECT_EQ(debt.id(), "debt-1");
    EXPECT_EQ(debt.name(), "Credit card");
    EXPECT_EQ(debt.type(), DebtType::CreditCard);
    EXPECT_EQ(debt.outstandingBalance(), Money::fromPaise(250'000));
    EXPECT_EQ(debt.annualInterestRate(), InterestRate::fromBasisPoints(1'850));
    EXPECT_EQ(debt.minimumPayment(), Money::fromPaise(10'000));
    EXPECT_EQ(debt.dueDay(), 15);
}

TEST(DebtTests, AllowsNoDueDay)
{
    EXPECT_EQ(makeValidDebt(std::nullopt).dueDay(), std::nullopt);
}

TEST(DebtTests, AllowsZeroInterestAndMinimumPayment)
{
    const Debt debt{
        "debt-2", "Interest free loan", DebtType::InformalLoan,
        Money::fromPaise(1), InterestRate::fromBasisPoints(0),
        Money::fromPaise(0)};

    EXPECT_TRUE(debt.annualInterestRate().isZero());
    EXPECT_TRUE(debt.minimumPayment().isZero());
}

TEST(DebtTests, RejectsEmptyId)
{
    EXPECT_THROW(
        Debt("", "Name", DebtType::Other, Money::fromPaise(1),
             InterestRate::fromBasisPoints(0), Money::fromPaise(0)),
        std::invalid_argument);
}

TEST(DebtTests, RejectsEmptyName)
{
    EXPECT_THROW(
        Debt("id", "", DebtType::Other, Money::fromPaise(1),
             InterestRate::fromBasisPoints(0), Money::fromPaise(0)),
        std::invalid_argument);
}

TEST(DebtTests, RejectsNonPositiveOutstandingBalance)
{
    for (const auto balance : {Money::fromPaise(-2), Money::fromPaise(-1)})
    {
        EXPECT_THROW(Debt("id", "Name", DebtType::Other, balance, InterestRate::fromBasisPoints(0), Money::fromPaise(0)),std::invalid_argument);
    }
}

TEST(DebtTests, RejectsNegativeMinimumPayment)
{
    EXPECT_THROW(
        Debt("id", "Name", DebtType::Other, Money::fromPaise(1),
             InterestRate::fromBasisPoints(0), Money::fromPaise(-1)),
        std::invalid_argument);
}

TEST(DebtTests, AcceptsDueDayBoundaries)
{
    EXPECT_EQ(makeValidDebt(1).dueDay(), 1);
    EXPECT_EQ(makeValidDebt(31).dueDay(), 31);
}

TEST(DebtTests, RejectsDueDayOutsideCalendarRange)
{
    EXPECT_THROW(makeValidDebt(0), std::invalid_argument);
    EXPECT_THROW(makeValidDebt(32), std::invalid_argument);
}
