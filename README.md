# Transaction Processing System

## Project Description

This project is a C-based bank account transaction processing system. It uses a random-access binary file, `credit.dat`, to store account records and allows the user to create, view, update, and delete account information.

The project demonstrates file handling, structures, random-access file processing, input validation, and transaction management in C.

## Features

- Create a formatted account report in `accounts.txt`
- List all active accounts on screen
- Search account details by account number
- Check account balance
- Deposit money into an account
- Withdraw money from an account
- Prevent withdrawal when balance is insufficient
- Add a new account
- Delete an existing account
- Count total active accounts
- Display accounts below minimum balance
- Save transaction history in `transactions.txt`
- Validate account numbers and user input

## Files Included

| File Name | Description |
|---|---|
| `trans.c` | Main C source code |
| `credit.dat` | Binary random-access account data file |
| `accounts.txt` | Generated text report of accounts |
| `transactions.txt` | Transaction history log |
| `README.md` | Project documentation |

## Account Structure

Each account stores:

- Account number
- Last name
- First name
- Balance

The program supports account numbers from `1` to `100`.

## Menu Options

```text
1  - create accounts.txt file
2  - list all accounts
3  - search account
4  - balance enquiry
5  - deposit amount
6  - withdraw amount
7  - add a new account
8  - delete an account
9  - count active accounts
10 - show low-balance accounts
11 - end program
```

## How to Compile

Open terminal or PowerShell in the project folder and run:

```bash
gcc -Wall -Wextra -std=c11 trans.c -o trans.exe
```

If using Replit or Linux:

```bash
gcc -Wall -Wextra -std=c11 trans.c -o trans
```

## How to Run

On Windows:

```powershell
.\trans.exe
```

On Replit or Linux:

```bash
./trans
```

## Sample Testing

### List all accounts

Choose:

```text
2
```

### Search account

Choose:

```text
3
```

Then enter an account number, for example:

```text
1
```

### Deposit amount

Choose:

```text
5
```

Then enter:

```text
Account number: 1
Deposit amount: 100
```

### Withdraw amount

Choose:

```text
6
```

Then enter:

```text
Account number: 1
Withdrawal amount: 50
```

### Generate account report

Choose:

```text
1
```

This creates or updates `accounts.txt`.

## Error Handling Added

- Invalid account numbers are rejected
- Account numbers must be between `1` and `100`
- Deposit and withdrawal amounts must be greater than zero
- Withdrawal is cancelled if the balance is insufficient
- Duplicate accounts cannot be created
- Missing accounts cannot be searched, updated, or deleted

## Transaction History

The program records important actions in `transactions.txt`, including:

- Account creation
- Deposit
- Withdrawal
- Account deletion

Each log entry includes the date and time, account number, transaction type, amount, old balance, and new balance.

## Conclusion

This project improves the basic transaction processing system by adding major banking features, better input validation, report generation, and transaction history. It is suitable for demonstrating C file handling and random-access record processing.
