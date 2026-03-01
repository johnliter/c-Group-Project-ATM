// This Account class is to be used with the ATM and BankCard
// Holds a monetary balance, adjustable by a BankCard object,
// and has a type (checking or savings) and an account number.

#include <iostream>
#include "cardaccounts.h"
using namespace std;

class Account {
    public:
        Account() {
            // Set default account type, number, and name based on user input
            SetAccountType();
            accName = (accountType == AccountType::CHECKING ? "Checking ..." : "Savings ...") + accNumber.substr(-5, -1);
        };
    private:
        AccountType accountType;
        string accNumber;
        string accName;
        double accBalance = 0.00;
        void SetAccountType();
};

enum class AccountType {
    CHECKING,
    SAVINGS
};

void Account::SetAccountType() {
    cout << "1) Checking\n2) Savings\nSelect account type to add: ";

    int choice;
    bool validChoice = false;

    // Validate user input for account type selection
    while (!validChoice) {
        cin >> choice;
        if (choice == 1 || choice == 2) {
            validChoice = true;
        } else {
            cout << "Invalid choice. Please select 1 for Checking or 2 for Savings: ";
        }
    }
    
    // Set account type and generate account number based on choice (checking starts with 1, savings starts with 2)
    AccountType accountType;
    if (choice == 1) {
        accountType = AccountType::CHECKING;
        accNumber = to_string(1000000 + rand() % 999999);
    } else {
        accountType = AccountType::SAVINGS;
        accNumber = to_string(2000000 + rand() % 999999);
    }
}

