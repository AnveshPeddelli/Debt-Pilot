# DebtPilot User Guide

## What DebtPilot helps you decide

If you have several credit-card payments, DebtPilot helps you explore:

- Which debt should receive your extra money first?
- How long could repayment take?
- How much interest could you pay?
- Would the snowball or avalanche strategy work better for your debts?

DebtPilot is currently a command-line planning tool. It does not connect to a bank, move money, or make payments for you.

## Before you begin

You will need:

- A Windows, macOS, or Linux computer.
- A built copy of DebtPilot supplied by the project owner, or the development tools described in the main README.
- The latest details for every credit card you want to include:
  - A unique label
  - Current outstanding balance
  - Annual interest rate
  - Current minimum monthly payment
- The extra amount you can reliably contribute every month.

Use figures from your latest statements where possible. Do not share a completed debt file publicly because it may contain sensitive financial information.

## Important terms

### Minimum payment

The amount you must pay toward a debt for the current month.

### Extra payment

Money available after covering the minimum payments on all active debts.

### Monthly budget

DebtPilot calculates this as:

```text
total active minimum payments + monthly extra payment
```

### Snowball strategy

Minimum payments go to every active debt, while extra money goes to the debt with the smallest outstanding balance. This can produce earlier visible wins.

### Avalanche strategy

Minimum payments go to every active debt, while extra money goes to the debt with the highest annual interest rate. This usually aims to minimize interest.

## Step 1: Generate a debt file

Open a terminal in the directory containing the DebtPilot executable.

On Windows:

```powershell
.\debtpilot_cli.exe generate-template 3 my-debts.json
```

On macOS or Linux:

```sh
./debtpilot_cli generate-template 3 my-debts.json
```

Replace `3` with your number of debts. DebtPilot creates `my-debts.json`, which you can edit with a plain-text editor.

If the project owner has given you a blank template, you can use that instead.

## Step 2: Enter your information

A two-debt file looks like this:

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

### Field reference

| Field | Meaning | Example |
|---|---|---|
| `monthly_extra_payment_rupees` | Extra money available after all minimum payments | `"5000.00"` |
| `maximum_months` | Safety limit for the simulation | `1200` |
| `id` | A unique, non-sensitive label for the debt | `"card-a"` |
| `name` | A recognizable display name | `"Credit Card A"` |
| `type` | Debt type; currently only `credit_card` is accepted | `"credit_card"` |
| `outstanding_balance_rupees` | Current unpaid balance | `"50000.00"` |
| `annual_interest_basis_points` | Annual rate in basis points | `1800` |
| `minimum_payment_rupees` | Current required monthly payment | `"5000.00"` |

Money values must be written as quoted strings.

### Converting an interest rate

One percentage point equals 100 basis points:

| Annual rate | Basis points |
|---:|---:|
| 0% | `0` |
| 12% | `1200` |
| 18% | `1800` |
| 24% | `2400` |
| 36% | `3600` |

For example, enter an annual rate of 18.5% as `1850`.

## Step 3: Run the planner

On Windows:

```powershell
.\debtpilot_cli.exe run my-debts.json
```

On macOS or Linux:

```sh
./debtpilot_cli run my-debts.json
```

If the file is stored elsewhere, provide its full or relative path.

## Step 4: Read the result

DebtPilot first shows:

- Minimum required monthly budget
- Configured extra payment
- Final monthly budget

It then compares:

- Payoff duration under each strategy
- Estimated total interest under each strategy
- Recommended strategy
- Difference in interest
- Difference in duration

The recommendation is based on the lower estimated interest cost. When interest is equal, the shorter plan wins. If both results are equal, DebtPilot recommends avalanche.

## Step 5: Interpret the recommendation carefully

Treat the result as a planning estimate, not an instruction from your lender.

Before paying:

1. Confirm the current amount due and due date on every statement.
2. Make at least the lender-required payment on every debt.
3. Check whether early or extra payments have special conditions.
4. Confirm how your lender applies an extra payment.
5. Keep an emergency buffer rather than committing money you may immediately need.

DebtPilot's current console output compares strategies, but it does not yet print its internal month-by-month allocation schedule. You must still use your current statements when deciding actual payment amounts.

## Trying different scenarios

You can copy the JSON file and change `monthly_extra_payment_rupees` to explore questions such as:

- What happens if I can add Rs. 2,000 per month?
- How much sooner could I finish with an additional Rs. 5,000?
- Is avalanche meaningfully cheaper for my debts?
- Is the difference small enough that I would prefer snowball's earlier wins?

Use different filenames so you can compare results:

```text
plan-extra-2000.json
plan-extra-5000.json
plan-extra-10000.json
```

## Troubleshooting

### `Could not open file`

Check the filename and its path. If the name contains spaces, surround it with quotes.

```powershell
.\debtpilot_cli.exe run "My Debt Plan.json"
```

### `Invalid JSON`

Common causes include:

- A missing comma
- A missing quotation mark
- An extra comma after the final item
- A missing opening or closing brace

Use the generated template as the starting point.

### `Unsupported debt type`

The current version accepts only:

```json
"type": "credit_card"
```

### `Debt IDs must be unique`

Give every debt a different `id`, such as `card-a`, `card-b`, and `card-c`.

### `Available budget is below total minimum payments`

Check that the balances and minimum payments are correct. The planner must be able to cover every active minimum payment.

### `Portfolio was not repaid within the maximum month limit`

The plan did not finish before `maximum_months`. Check the entered rates, balances, and payments. You can also raise the limit when the scenario is otherwise valid.

## Privacy

A completed JSON file can reveal balances, rates, and lender names.

- Do not commit your personal debt file to a public GitHub repository.
- Use labels such as `card-a` instead of account or card numbers.
- Do not include passwords, PINs, CVVs, complete card numbers, or banking credentials.
- Remove or replace personal values before sharing an example or bug report.

## Current calculation assumptions

DebtPilot assumes:

- Interest is calculated once per month as annual rate divided by 12.
- Interest is charged before the monthly payment.
- Rates and minimum payments do not change during the plan.
- The same overall repayment budget is available every month.
- No new spending or borrowing occurs.
- There are no fees, penalties, taxes, promotional periods, or special lender rules.

Real statements can differ because lenders may use daily balances, different rounding rules, changing minimum payments, statement cycles, and fees.

## Share feedback

When trying DebtPilot, please avoid sending real account details. Share sanitized examples and answer:

1. Was creating or editing the JSON file understandable?
2. Did you know what information to enter?
3. Was the comparison easy to understand?
4. Did the result help you choose a strategy?
5. What information did you expect but not receive?
6. Would exact month-by-month payment instructions make it more useful?
7. Which part felt confusing or unsafe?
8. Would you use it again after your balances changed?

When reporting a problem, include:

- Your operating system
- The command you ran
- The exact error message
- A sanitized input file with invented names and balances
- What you expected to happen

## Disclaimer

DebtPilot is an early planning tool. Its projections are estimates and may not match lender calculations. It does not provide financial advice, execute payments, or replace official statements and professional guidance.
