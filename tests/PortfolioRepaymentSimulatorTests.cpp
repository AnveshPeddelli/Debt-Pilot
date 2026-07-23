#include "debtpilot/PortfolioRepaymentSimulator.hpp"

#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

using debtpilot::Debt;
using debtpilot::DebtType;
using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::PortfolioRepaymentSimulator;
using debtpilot::RepaymentStrategy;

namespace
{
    Debt createDebt(const std::string& id, std::int64_t balacncePaise, std::int32_t annualBasisPoints, std::int64_t minimumPaymentPaise)
    {
        return Debt{id, id, DebtType::CreditCard, Money::fromPaise(balacncePaise), InterestRate::fromBasisPoints(annualBasisPoints), Money::fromPaise(minimumPaymentPaise)};
    }
}

//Empty portfolio
TEST(PortfolioRepaymentSimulatorTest, EmptyPortfolioProducesEmptyPlan)
{
    const std::vector<Debt> debts;
    const auto plan = PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(0), RepaymentStrategy::Snowball);

    EXPECT_EQ(plan.totalMonths(), 0);
    EXPECT_EQ(plan.totalInterest().paise(), 0);
    EXPECT_EQ(plan.totalPaid().paise(), 0);
    EXPECT_TRUE(plan.monthlyResults().empty());
    EXPECT_TRUE(plan.payoffMonths().empty());
}

//Repays two zero-interest debts
TEST(PortfolioRepaymentSimulatorTest, RepaysMultipleDebts)
{
    const std::vector<Debt> debts{
        createDebt("small", 100'000, 0, 25'000),
        createDebt("large", 200'000, 0, 25'000)
    };
    const auto plan = PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(100'000), RepaymentStrategy::Snowball);

    EXPECT_EQ(plan.totalMonths(), 3);
    EXPECT_EQ(plan.totalInterest().paise(), 0);
    EXPECT_EQ(plan.totalPaid().paise(), 300'000);

    ASSERT_EQ(plan.payoffMonths().count("small"), 1);
    ASSERT_EQ(plan.payoffMonths().count("large"), 1);

    EXPECT_EQ(plan.payoffMonths().at("small"), 2);
    EXPECT_EQ(plan.payoffMonths().at("large"), 3);
}

//Tracks portfolio interest
TEST(PortfolioRepaymentSimulatorTest, AccumulatesInterestAcrossAllDebts)
{
    const std::vector<Debt> debts{
        createDebt("a", 100'000, 1'200, 50'000),
        createDebt("b", 100'000, 1'200, 50'000)
    };

    const auto plan = PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(120'000), RepaymentStrategy::Avalanche);

    EXPECT_GT(plan.totalInterest().paise(), 0);
    EXPECT_EQ(plan.totalPaid(), Money::fromPaise(200'000) + plan.totalInterest());
}

//Snowball clears smallest balance first
TEST(PortfolioRepaymentSimulatorTest, SnowballClearSmallestDebtFirst)
{
    const std::vector<Debt> debts{
        createDebt("large", 500'000, 0, 50'000),
        createDebt("small", 100'000, 0, 50'000)
    };

    const auto plan = PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(200'000), RepaymentStrategy::Snowball);

    EXPECT_LT(plan.payoffMonths().at("small"), plan.payoffMonths().at("large"));
}

//Avalanche prioritizes highest rate
TEST(PortfolioRepaymentSimulatorTest, AvalancheClearsHighestRateDebtFirst)
{
    const std::vector<Debt> debts{
        createDebt("low-rate", 200'000, 1'200, 50'000),
        createDebt("high-rate", 200'000, 3'600, 50'000)
    };

    const auto plan = PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(200'000), RepaymentStrategy::Avalanche);

    EXPECT_LE(plan.payoffMonths().at("high-rate"), plan.payoffMonths().at("low-rate"));
}

//Duplicate IDs rejected
TEST(PortfolioRepaymentSimulatorTest, RejectDuplicateIds)
{
    const std::vector<Debt> debts{
        createDebt("duplicate", 100'000, 0, 50'000),
        createDebt("duplicate", 200'000, 0, 50'000)
    };

    EXPECT_THROW(PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(100'000), RepaymentStrategy::Snowball), std::invalid_argument);
}

//Maximum month limit
TEST(PortfolioRepaymentSimulatorTest, StopAtMaximumLimit)
{
    const std::vector<Debt> debts{
        createDebt("large", 1'000'000, 0, 1)
    };

    EXPECT_THROW(PortfolioRepaymentSimulator::simulate(debts, Money::fromPaise(1), RepaymentStrategy::Snowball, 12), std::invalid_argument);
}
