// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MIN_BALANCE 500.0

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};                        // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void listRecords(FILE *readPtr);
void searchRecord(FILE *fPtr);
void balanceInquiry(FILE *fPtr);
void depositRecord(FILE *fPtr);
void withdrawRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void accountCount(FILE *readPtr);
void lowBalanceReport(FILE *readPtr);
void printHeader(void);
void printClient(struct clientData client);
int readClient(FILE *fPtr, unsigned int accountNum, struct clientData *client);
int writeClient(FILE *fPtr, struct clientData client);
int isValidAccount(unsigned int accountNum);
void logTransaction(const char *type, struct clientData client, double amount, double oldBalance);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice
    (void)argc;

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 11)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            listRecords(cfPtr);
            break;
        case 3:
            searchRecord(cfPtr);
            break;
        case 4:
            balanceInquiry(cfPtr);
            break;
        case 5:
            depositRecord(cfPtr);
            break;
        case 6:
            withdrawRecord(cfPtr);
            break;
        case 7:
            newRecord(cfPtr);
            break;
        case 8:
            deleteRecord(cfPtr);
            break;
        case 9:
            accountCount(cfPtr);
            break;
        case 10:
            lowBalanceReport(cfPtr);
            break;
        default:
            puts("Incorrect choice");
            break;
        } // end switch
    }     // end while

    fclose(cfPtr); // fclose closes the file
} // end main

// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; // accounts.txt file pointer
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    puts("accounts.txt created successfully.");
} // end function textFile

// display all account records on screen
void listRecords(FILE *readPtr)
{
    int found = 0;
    struct clientData client = {0, "", "", 0.0};

    rewind(readPtr);
    printHeader();

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printClient(client);
            found = 1;
        }
    }

    if (!found)
    {
        puts("No active accounts found.");
    }
} // end function listRecords

// search and display one account
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account number to search ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client) || client.acctNum == 0)
    {
        printf("Account #%u does not exist.\n", accountNum);
        return;
    }

    printHeader();
    printClient(client);
} // end function searchRecord

// display current balance for one account
void balanceInquiry(FILE *fPtr)
{
    unsigned int accountNum;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account number for balance enquiry ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client) || client.acctNum == 0)
    {
        printf("Account #%u does not exist.\n", accountNum);
        return;
    }

    printf("Current balance for account #%u is %.2f\n", client.acctNum, client.balance);
} // end function balanceInquiry

// add money to an account
void depositRecord(FILE *fPtr)
{
    unsigned int accountNum;
    double amount;
    double oldBalance;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account number to deposit into ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client) || client.acctNum == 0)
    {
        printf("Account #%u does not exist.\n", accountNum);
        return;
    }

    printf("%s", "Enter deposit amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        puts("Deposit amount must be greater than zero.");
        return;
    }

    oldBalance = client.balance;
    client.balance += amount;

    if (writeClient(fPtr, client))
    {
        logTransaction("DEPOSIT", client, amount, oldBalance);
        printf("Deposit successful. New balance: %.2f\n", client.balance);
    }
} // end function depositRecord

// remove money from an account
void withdrawRecord(FILE *fPtr)
{
    unsigned int accountNum;
    double amount;
    double oldBalance;
    struct clientData client = {0, "", "", 0.0};

    printf("%s", "Enter account number to withdraw from ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client) || client.acctNum == 0)
    {
        printf("Account #%u does not exist.\n", accountNum);
        return;
    }

    printf("%s", "Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        puts("Withdrawal amount must be greater than zero.");
        return;
    }

    if (amount > client.balance)
    {
        puts("Insufficient balance. Withdrawal cancelled.");
        return;
    }

    oldBalance = client.balance;
    client.balance -= amount;

    if (writeClient(fPtr, client))
    {
        logTransaction("WITHDRAW", client, amount, oldBalance);
        printf("Withdrawal successful. New balance: %.2f\n", client.balance);
        if (client.balance < MIN_BALANCE)
        {
            printf("Warning: balance is below minimum balance %.2f\n", MIN_BALANCE);
        }
    }
} // end function withdrawRecord

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    struct clientData blankClient = {0, "", "", 0};
    unsigned int accountNum;

    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client) || client.acctNum == 0)
    {
        printf("Account %u does not exist.\n", accountNum);
        return;
    }

    blankClient.acctNum = accountNum;
    if (writeClient(fPtr, blankClient))
    {
        logTransaction("DELETE", client, 0.0, client.balance);
        printf("Account #%u deleted successfully.\n", accountNum);
    }
} // end function deleteRecord

// create and insert record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("%s", "Enter new account number ( 1 - 100 ): ");
    if (scanf("%u", &accountNum) != 1 || !isValidAccount(accountNum))
    {
        puts("Invalid account number.");
        return;
    }

    if (!readClient(fPtr, accountNum, &client))
    {
        puts("Unable to read account record.");
        return;
    }

    if (client.acctNum != 0)
    {
        printf("Account #%u already contains information.\n", client.acctNum);
        return;
    }

    printf("%s", "Enter lastname, firstname, opening balance\n? ");
    if (scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance) != 3 || client.balance < 0)
    {
        puts("Invalid account information.");
        return;
    }

    client.acctNum = accountNum;
    if (writeClient(fPtr, client))
    {
        logTransaction("CREATE", client, client.balance, 0.0);
        printf("Account #%u created successfully.\n", accountNum);
    }
} // end function newRecord

// display the total number of active accounts
void accountCount(FILE *readPtr)
{
    unsigned int count = 0;
    struct clientData client = {0, "", "", 0.0};

    rewind(readPtr);
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            count++;
        }
    }

    printf("Total active accounts: %u\n", count);
} // end function accountCount

// display accounts below minimum balance
void lowBalanceReport(FILE *readPtr)
{
    int found = 0;
    struct clientData client = {0, "", "", 0.0};

    rewind(readPtr);
    printf("Accounts below minimum balance %.2f:\n", MIN_BALANCE);
    printHeader();

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0 && client.balance < MIN_BALANCE)
        {
            printClient(client);
            found = 1;
        }
    }

    if (!found)
    {
        puts("No accounts are below the minimum balance.");
    }
} // end function lowBalanceReport

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("%s", "\nEnter your choice\n"
                 "1  - create accounts.txt file\n"
                 "2  - list all accounts\n"
                 "3  - search account\n"
                 "4  - balance enquiry\n"
                 "5  - deposit amount\n"
                 "6  - withdraw amount\n"
                 "7  - add a new account\n"
                 "8  - delete an account\n"
                 "9  - count active accounts\n"
                 "10 - show low-balance accounts\n"
                 "11 - end program\n? ");

    if (scanf("%u", &menuChoice) != 1)
    {
        return 0;
    }

    return menuChoice;
} // end function enterChoice

// display account table heading
void printHeader(void)
{
    printf("%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
} // end function printHeader

// display one account
void printClient(struct clientData client)
{
    printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
} // end function printClient

// read one account record from file
int readClient(FILE *fPtr, unsigned int accountNum, struct clientData *client)
{
    long offset;

    if (!isValidAccount(accountNum))
    {
        return 0;
    }

    offset = (long)(accountNum - 1) * (long)sizeof(struct clientData);
    fseek(fPtr, offset, SEEK_SET);
    return fread(client, sizeof(struct clientData), 1, fPtr) == 1;
} // end function readClient

// write one account record to file
int writeClient(FILE *fPtr, struct clientData client)
{
    long offset;

    if (!isValidAccount(client.acctNum))
    {
        puts("Invalid account number.");
        return 0;
    }

    offset = (long)(client.acctNum - 1) * (long)sizeof(struct clientData);
    fseek(fPtr, offset, SEEK_SET);
    if (fwrite(&client, sizeof(struct clientData), 1, fPtr) != 1)
    {
        puts("Unable to write account record.");
        return 0;
    }

    fflush(fPtr);
    return 1;
} // end function writeClient

// validate account number range
int isValidAccount(unsigned int accountNum)
{
    return accountNum >= 1 && accountNum <= MAX_ACCOUNTS;
} // end function isValidAccount

// save account activity to a text history file
void logTransaction(const char *type, struct clientData client, double amount, double oldBalance)
{
    FILE *logPtr;
    time_t now;
    char timestamp[30];

    logPtr = fopen("transactions.txt", "a");
    if (logPtr == NULL)
    {
        puts("Warning: transaction history could not be opened.");
        return;
    }

    now = time(NULL);
    strncpy(timestamp, ctime(&now), sizeof(timestamp) - 1);
    timestamp[sizeof(timestamp) - 1] = '\0';
    timestamp[strcspn(timestamp, "\n")] = '\0';

    fprintf(logPtr, "%s | %-8s | Acct: %-3u | %-14s %-9s | Amount: %10.2f | Old: %10.2f | New: %10.2f\n",
            timestamp, type, client.acctNum, client.lastName, client.firstName, amount, oldBalance, client.balance);

    fclose(logPtr);
} // end function logTransaction
