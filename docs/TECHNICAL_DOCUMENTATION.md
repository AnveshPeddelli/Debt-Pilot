# DebtPilot Technical Documentation

## 1. Purpose

DebtPilot is a C++20 repayment-planning engine with a command-line interface. It models a portfolio of credit-card debts under a fixed monthly repayment budget and compares debt snowball and debt avalanche strategies.

The motivating problem is:

> Given several debts and a limited monthly budget, determine which debt to prioritize, how payments are allocated, and when the portfolio is projected to be repaid.

The current release exposes strategy-level results through the CLI. The core model also records monthly debt snapshots and allocations that can support more detailed reporting later.

## 2. Scope

### In scope

- Exact currency representation in integer paise
- Annual interest rates represented in basis points
- Monthly interest estimation
- Minimum and extra payment allocation
- Snowball and avalanche prioritization
- Month-by-month portfolio simulation
- Strategy comparison
- JSON input and template generation
- Console summary reporting

### Outside the current scope

- Bank connectivity or payment execution
- User accounts and persistent plan history
- Statement import
- Daily interest or average-daily-balance models
- Variable rates and changing minimum-payment formulas
- Fees, penalties, taxes, and promotional periods
- Additional borrowing during a plan
- Graphical or web interfaces
- Financial advice

## 3. Architecture

DebtPilot separates the repayment domain from input and presentation:

```text
JSON file
   |
   v
DebtFileManager
   |
   v
DebtPlanningRequest
   |
   v
DebtPlanningService
   |
   +--> PortfolioRepaymentSimulator (Snowball)
   |
   +--> PortfolioRepaymentSimulator (Avalanche)
               |
               +--> PortfolioPaymentAllocator
               |        |
               |        +--> DebtPrioritizer
               |
               +--> MonthlyRepaymentCalculator
                        |
                        +--> MonthlyInterestCalculator
   |
   v
StrategyComparisonResult
   |
   v
ConsoleReporter
```

### Directory layout

```text
core/
  include/debtpilot/   Public domain and service interfaces
  src/                 Core implementations

cli/
  include/debtpilot/cli/  CLI-specific interfaces
  src/                    JSON and console implementations
  main.cpp                Command dispatch and orchestration

tests/                 GoogleTest suites
docs/                  Requirements and project documentation
```

## 4. Domain model

### Money

`Money` stores currency as a signed 64-bit number of paise. This avoids the representation errors associated with binary floating-point currency.

Key behavior includes:

- Construction from paise
- Addition and subtraction
- Ordering and equality
- Zero checks
- Overflow-aware arithmetic

### InterestRate

`InterestRate` represents an annual interest rate in basis points.

```text
100 basis points = 1 percentage point
1800 basis points = 18% annually
```

### Debt

A debt contains:

- Unique ID
- Display name
- Debt type
- Outstanding balance
- Annual interest rate
- Minimum payment

The JSON loader currently accepts only `DebtType::CreditCard`.

### DebtPlanningRequest

The request combines:

- A debt portfolio
- Fixed monthly budget
- Maximum simulation length

### RepaymentStrategy

Two strategies are supported:

- `Snowball`
- `Avalanche`

### PortfolioRepaymentPlan

A completed simulation contains:

- Monthly portfolio results
- Payoff month for each debt
- Total interest paid
- Total amount paid
- Total payoff duration

### StrategyComparisonResult

The comparison contains:

- Snowball plan
- Avalanche plan
- Recommended strategy
- Absolute interest difference
- Absolute duration difference

## 5. Input format

The CLI reads a JSON object:

```json
{
    "monthly_extra_payment_rupees": "5000.00",
    "maximum_months": 1200,
    "debts": [
        {
            "id": "card-a",
            "name": "Credit Card A",
            "type": "credit_card",
            "outstanding_balance_rupees": "50000.00",
            "annual_interest_basis_points": 1800,
            "minimum_payment_rupees": "5000.00"
        }
    ]
}
```

### Validation rules

- `monthly_extra_payment_rupees` must be a non-negative money string.
- `maximum_months` must be greater than zero.
- `debts` must be a non-empty array when loaded through the CLI.
- Debt IDs must be unique during portfolio simulation.
- Money strings support zero, one, or two decimal places.
- Negative money values are rejected by the JSON parser.
- Annual interest is supplied as a whole number of basis points.
- Only `credit_card` is currently accepted as a debt type.

The generated template uses a default simulation limit of 1,200 months.

## 6. Planning workflow

For the `run` command:

1. `DebtFileManager` parses and validates the JSON document.
2. `MinimumBudgetCalculator` totals the current required minimum payments.
3. The configured extra payment is added to form the fixed monthly budget.
4. `DebtPlanningService` requests a comparison.
5. `StrategyComparisonEngine` simulates snowball and avalanche independently.
6. The engine recommends the plan with lower total interest.
7. If total interest is equal, it recommends the plan with fewer months.
8. If both metrics are equal, avalanche remains the default.
9. `ConsoleReporter` prints the comparison.

## 7. Payment allocation algorithm

For each simulated month:

1. Ignore already-repaid debts.
2. Calculate each active debt's monthly interest.
3. Calculate the amount due as opening balance plus interest.
4. Reserve the lesser of the configured minimum payment and amount due.
5. Reject the plan if the monthly budget cannot cover all reserved minimums.
6. Sort debts according to the selected strategy.
7. Apply the remaining budget in priority order.
8. Cap each allocation at the remaining amount due.
9. Return any budget that is not needed because the portfolio is nearly repaid.

### Snowball ordering

1. Lowest outstanding balance
2. Highest annual interest rate when balances tie
3. Lexicographically lowest debt ID when both tie

### Avalanche ordering

1. Highest annual interest rate
2. Lowest outstanding balance when rates tie
3. Lexicographically lowest debt ID when both tie

## 8. Monthly repayment calculation

Monthly interest is estimated as:

```text
interest = opening balance in paise * annual basis points / 120000
```

The denominator combines:

```text
100 basis points per percentage point
* 100 percentage points per whole rate
* 12 months
= 120000
```

The result is rounded to the nearest paise, with half-paise values rounded upward.

For each debt:

```text
amount due       = opening balance + monthly interest
actual payment   = min(requested payment, amount due)
closing balance  = amount due - actual payment
principal repaid = max(actual payment - interest, 0)
```

## 9. Simulation termination and safeguards

The simulator stops successfully when every outstanding balance is zero.

It rejects or stops scenarios when:

- The monthly budget is negative.
- The maximum month count is zero.
- Debt IDs are duplicated.
- The portfolio exceeds the maximum month limit.
- Nothing is allocated while debt remains.
- The allocated payments fail to reduce any balance.
- Allocation and debt counts become inconsistent.

These safeguards prevent silent infinite simulations and detect invalid internal states.

## 10. Output

The CLI currently prints:

- Minimum required monthly budget
- Configured extra payment
- Final monthly budget
- Payoff months for both strategies
- Total interest for both strategies
- Recommended strategy
- Absolute interest difference
- Absolute duration difference

The core plan contains richer monthly information, including opening balance, interest, payment, principal repaid, and closing balance for each debt. That information is not yet exposed in the CLI comparison report.

## 11. Build and dependencies

### Requirements

- CMake 3.20+
- C++20-compatible compiler
- Git/network access for the initial dependency fetch

### Dependencies

- nlohmann/json 3.12.0
- GoogleTest 1.17.0 when tests are enabled

### Configure and build

```sh
cmake -S . -B build
cmake --build build --config Release
```

Disable tests when required:

```sh
cmake -S . -B build -DDEBTPILOT_BUILD_TESTS=OFF
cmake --build build --config Release
```

### Run tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

Strict compiler warnings are enabled:

- MSVC: `/W4`, `/permissive-`, `/Zc:__cplusplus`
- GCC/Clang: `-Wall`, `-Wextra`, `-Wpedantic`, `-Wconversion`, `-Wsign-conversion`

## 12. Test coverage

The repository contains tests for:

- `Money`
- `InterestRate`
- `Debt`
- Monthly interest calculation
- Monthly repayment calculation
- Repayment schedule generation
- Debt prioritization
- Portfolio payment allocation
- Portfolio repayment simulation
- Strategy comparison
- Planning service orchestration

The tests cover normal calculations as well as invalid inputs, overflow boundaries, repayment completion, allocation consistency, duplicate IDs, insufficient budgets, strategy ordering, and accounting identities such as:

```text
total paid = original principal + total interest
```

Passing unit tests demonstrate consistency with the implemented model. They do not establish that the simplified model exactly matches every lender's calculation rules.

## 13. Known limitations and risks

### Model accuracy

Many credit-card issuers calculate interest using daily balances and statement-specific rules. DebtPilot's monthly model can therefore diverge from real statements.

### Static minimum payments

The minimum payment entered for each debt is treated as fixed for the simulation, although lenders may recalculate it every statement cycle.

### Static rates and budget

Rates and the overall monthly budget are assumed constant. Variable rates, missed payments, emergency expenses, and windfalls are not represented.

### Recommendation objective

The recommendation prioritizes estimated interest and then duration. It does not model behavioral preference, liquidity needs, credit utilization, delinquency, negotiated settlements, or risk.

### Interface gap

The engine produces monthly snapshots, but the current CLI does not turn them into month-by-month user instructions. This is the most important gap relative to the motivating problem statement.

### Financial sensitivity

Input files may contain private financial data. The application does not currently provide encryption or managed storage. Users are responsible for protecting their files.

## 14. Extension points

The core/CLI separation supports future work such as:

- Detailed schedule reporting
- CSV or PDF export
- Interactive CLI input
- Desktop, web, or mobile interfaces
- Actual-versus-planned tracking
- Replanning from updated balances
- Additional debt types
- Alternative interest and minimum-payment policies
- Scenario comparison across multiple budgets

New debt types should not be added merely as labels. Each type may require its own interest, payment, fee, and prioritization rules.

## 15. Recommended next milestone

The next milestone should close the gap between strategy comparison and actionable planning:

> For every simulated month, show the exact payment assigned to every active debt, identify the priority debt, show its projected payoff month, and state the projected debt-free month.

That output should clearly distinguish an estimate from an actual statement amount and should be tested against manually verified scenarios.

## 16. Security and privacy guidance

- Never store passwords, PINs, CVVs, tokens, or banking credentials.
- Avoid using complete card or account numbers as debt IDs.
- Keep personal input files out of public version control.
- Sanitize examples before submitting issues.
- Treat any future bank integration as a separate security-sensitive feature requiring threat modeling and appropriate compliance review.

## 17. Disclaimer

DebtPilot is an early planning tool. It does not provide financial advice, execute payments, or guarantee lender-accurate results. Users should verify all actual amounts, due dates, and payment rules against official statements.
