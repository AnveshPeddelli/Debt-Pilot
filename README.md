# DebtPilot

DebtPilot is a C++ command-line debt repayment planner. It helps answer three practical questions:

> Which debt should I pay first, how much should go to each debt, and when will I be debt-free?

The current version compares two common repayment strategies using a fixed monthly budget:

- **Debt snowball** — directs extra money toward the smallest outstanding balance first.
- **Debt avalanche** — directs extra money toward the highest interest rate first.

DebtPilot simulates both strategies month by month, estimates their payoff duration and total interest, and recommends the strategy with the lower projected interest cost.

## Current capabilities

- Load multiple debts from a JSON file.
- Calculate the minimum required monthly budget.
- Add a configurable extra monthly payment.
- Allocate minimum payments across all active debts.
- Direct remaining money according to the selected repayment strategy.
- Roll freed payments into the remaining debts.
- Track monthly balances, payments, principal, and interest internally.
- Compare snowball and avalanche payoff results.
- Report the projected payoff duration, total interest, and estimated savings.
- Generate a starter JSON template from the command line.
- Perform monetary calculations in integer paise rather than floating-point currency.

DebtPilot currently supports credit-card debts and displays monetary values in Indian rupees.

## Documentation

- [User Guide](docs/USER_GUIDE.md) — setup, data entry, result interpretation, troubleshooting, privacy, and a feedback checklist
- [Technical Documentation](docs/TECHNICAL_DOCUMENTATION.md) — architecture, domain model, algorithms, validation, testing, limitations, and extension points

## Example result

```text
Minimum required monthly budget: Rs. 50000
Configured extra payment: Rs. 20000
Final monthly budget: Rs. 70000

===============================================
STRATEGY COMPARISION
===============================================
Metric                  Snowball          Avalanche
--------------------------------------------------------------------------
Payoff months           4                 4
Total interest          Rs. 8654.36       Rs. 8266.91

Recommended strategy : Avalanche
Interest difference : Rs. 387.45
Duration difference : 0 months
```

Results depend entirely on the debt information and extra payment supplied by the user.

## Requirements

- A C++20-compatible compiler
- CMake 3.20 or newer
- Git and an internet connection during the initial CMake configuration

CMake fetches the following dependencies:

- [nlohmann/json](https://github.com/nlohmann/json) 3.12.0
- [GoogleTest](https://github.com/google/googletest) 1.17.0 when tests are enabled

## Build

```sh
cmake -S . -B build
cmake --build build --config Release
```

Tests are enabled by default. To build without them:

```sh
cmake -S . -B build -DDEBTPILOT_BUILD_TESTS=OFF
cmake --build build --config Release
```

## Usage

The executable is normally located at:

- Windows multi-configuration builds: `build/cli/Release/debtpilot_cli.exe`
- Single-configuration builds: `build/cli/debtpilot_cli`

### Generate an input template

```sh
debtpilot_cli generate-template <debt-count> <output-file>
```

For example:

```sh
debtpilot_cli generate-template 3 my-debts.json
```

### Configure the debts

Money fields are strings expressed in rupees. Interest rates are annual basis points, where `1800` means 18.00% per year.

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
        },
        {
            "id": "card-b",
            "name": "Credit Card B",
            "type": "credit_card",
            "outstanding_balance_rupees": "90000.00",
            "annual_interest_basis_points": 2400,
            "minimum_payment_rupees": "7000.00"
        }
    ]
}
```

Each debt ID must be unique. Currently, `credit_card` is the only supported debt type.

### Run the comparison

```sh
debtpilot_cli run my-debts.json
```

The program calculates:

```text
monthly budget = active minimum payments + configured extra payment
```

It then holds that overall budget constant throughout each simulation, allowing money freed by a repaid debt to be redirected to the remaining debts.

## Run the tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

The test suite covers the currency and interest value objects, debt validation, interest and repayment calculations, payment allocation, portfolio simulation, strategy prioritization, and comparison behavior.

## Calculation assumptions

DebtPilot is a planning model, so its projections use simplified assumptions:

- Interest is estimated monthly as the outstanding balance multiplied by the annual rate divided by 12.
- Interest is rounded to the nearest paise, with half-paise values rounded upward.
- The configured interest rate and minimum payment remain unchanged during a simulation.
- Interest is applied before that month's payment.
- The same total repayment budget is available every month.
- No new purchases or additional borrowing occur.
- Fees, penalties, taxes, promotional periods, daily compounding, and statement/payment dates are not modeled.

Because lenders may calculate balances and interest differently, projected results may not exactly match bank statements.

## Current limitations

- Only credit-card debt is accepted by the JSON loader.
- The CLI prints a strategy summary rather than the detailed month-by-month allocations already produced by the core engine.
- Plans cannot yet be updated with actual payments or changing balances.
- There is no graphical interface, bank integration, notification system, or encrypted data store.
- This project is a planning aid, not financial advice and not a payment-processing application.

## Project structure

```text
core/          Domain model, calculations, allocation, and simulation
cli/           JSON input, commands, and console reporting
tests/         GoogleTest unit and integration tests
docs/          Project requirements documentation
```

The core is kept separate from the command-line interface so that other interfaces can be added later without rewriting the repayment engine.

## Roadmap

Near-term development is focused on making the plan directly actionable:

- Print exact payment instructions for every debt and every month.
- Show the payoff order and projected debt-free date.
- Support replanning when balances, rates, or the available budget change.
- Validate projections against representative statements and repayment scenarios.
- Improve usability before considering broader debt types or product integrations.

## Status

DebtPilot is currently an early personal tool and an actively evolving project. It is useful for exploring repayment strategies, but it should not yet be treated as a bank-accurate or production financial product.

## License

Licensed under the [Apache License 2.0](LICENSE).
