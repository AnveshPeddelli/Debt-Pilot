#include "debtpilot/DebtPrioritizer.hpp"

#include <gtest/gtest.h>

using debtpilot::Debt;
using debtpilot::DebtType;
using debtpilot::DebtPrioritizer;
using debtpilot::InterestRate;
using debtpilot::Money;
using debtpilot::RepaymentStrategy;

namespace
{
    Debt createDebt(const std::string& id, std::int64_t balancePaise, std::int32_t annualBasisPoints)
    {
        return Debt{id, id, DebtType::CreditCard, Money::fromPaise(balancePaise), InterestRate::fromBasisPoints(annualBasisPoints), Money::fromPaise(10'000)};
    }
}

//Snowball test
TEST(DebtPrioritizerTest, SnowballOrdersLowestBalanceFirst)
{
    const std::vector<Debt> debts{
        createDebt("large", 500'000, 1'200),
        createDebt("small", 100'000, 3'600),
        createDebt("medium", 300'000, 1'800)
    };

    const auto order = DebtPrioritizer::prioritize(debts, RepaymentStrategy::Snowball);

    EXPECT_EQ(debts[order[0]].id(), "small");
    EXPECT_EQ(debts[order[1]].id(), "medium");
    EXPECT_EQ(debts[order[2]].id(), "large"); 
}

//Avalanche test
TEST(DebtPrioritizerTest, AvalancheOrdersHighestinterestFirst)
{
    const std::vector<Debt>debts{
        createDebt("personal-loan", 500'000, 1'200),
        createDebt("credit-card", 100'000, 3'600),
        createDebt("car-loan", 300'000, 1'800)
    };

    const auto order = DebtPrioritizer::prioritize(debts, RepaymentStrategy::Avalanche);

    ASSERT_EQ(order.size(), 3);

    EXPECT_EQ(debts[order[0]].id(), "credit-card");
    EXPECT_EQ(debts[order[1]].id(), "car-loan");
    EXPECT_EQ(debts[order[2]].id(), "personal-loan");
}

//Snowball tie-breaker test
TEST(DebtPrioritizerTest, SnowballUsesHigherInterestForEqualBalances)
{
    const std::vector<Debt> debts{
        createDebt("low-rate", 100'000, 1'200),
        createDebt("high-rate", 100'000, 3'600)
    };

    const auto order = DebtPrioritizer::prioritize(debts, RepaymentStrategy::Snowball);

    ASSERT_EQ(order.size(), 2);
    EXPECT_EQ(debts[order[0]].id(), "high-rate");
}

//Avalanche tie-breaker test
TEST(DebtPrioritizerTest, AvalancheUsesLowerBalanceForEqualRates)
{
    const std::vector<Debt> debts{
        createDebt("large", 500'000, 1'800),
        createDebt("small", 100'000, 1'800)
    };

    const auto order = DebtPrioritizer::prioritize(debts, RepaymentStrategy::Avalanche);

    ASSERT_EQ(order.size(), 2);
    EXPECT_EQ(debts[order[0]].id(), "small");
}

//Empth collection test
TEST(DebtPrioritizerTest, EmptyDebtCollectionProducesEmptyOrder)
{
    const std::vector<Debt> debts;
    const auto order = DebtPrioritizer::prioritize(debts, RepaymentStrategy::Snowball);

    EXPECT_TRUE(order.empty());
}