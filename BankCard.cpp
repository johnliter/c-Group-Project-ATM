// This BankCard class digitally represents a physical card for the sake of 
// the overall bank atm simulation program. It holds cardholder information,
// and accounts with balances adjustable by card methods. Has card# and PIN.

#include <iostream>
#include <vector>
#include <algorithm>
#include "cardaccounts.h"
using namespace std;

class BankCard {
    public:
        BankCard() {
            // Generate a random card number and set basic info with input validation
            cardNumber = to_string(8008123400000000 + rand() % 99999999);
            SetName();
            SetPIN();

            // Asks for number of accounts to add to card and adds them
            AddAccounts();
        }
        void SetPIN();
        void Deposit(Account acc);
        void Withdraw(Account acc);
        void Transfer(Account acc1, Account acc2);
        
    private:
        vector<Account> accounts;
        string cardNumber;
        string holderName;
        string cardPIN;
        void SetName();
        void AddAccounts();                   
};

void BankCard::SetName() {
    // Set the cardholder's name with input validation to ensure it's not empty and contains only letters
    cout << "Enter cardholder name: ";
    
    string name;
    bool validName = false;

    while (!validName) {
        getline(cin, name);
        if (!name.empty() && all_of(name.begin(), name.end(), ::isalpha)) {
            validName = true;
        } else {
            cout << "Error. Please enter a valid name: ";
        }
    }
    holderName = name;
}

void BankCard::SetPIN() {
    // Set the card's PIN with input validation to ensure it's a 4-digit number
    cout << "Enter new 4-digit PIN: ";
    
    string newPIN;
    bool validPIN = false;

    while (!validPIN) {
        cin >> newPIN;
        if (newPIN.length() == 4 && all_of(newPIN.begin(), newPIN.end(), ::isdigit)) {
            validPIN = true;
        } else {
            cout << "Invalid PIN. Please enter a 4-digit number: ";
        }
    }

    cardPIN = newPIN;
}

void BankCard::AddAccounts() {
    // Adds user-specified # of accounts to the card
    cout << "How many accounts (1-3) would you like to add to this card? ";
            
            int numAccounts;
            bool validInput = false;

            // Validate user input for number of accounts
            while (!validInput) {
                cin >> numAccounts;
                if (numAccounts > 0 && numAccounts <= 3 && cin.good()) {
                    validInput = true;
                } else {
                    cout << "Invalid input. Amount must be between 1 and 3: ";
                }
            }

            // Add the specified number of accounts to the accounts vector
            for (int i = 0; i < numAccounts; i++) {
                accounts.push_back(Account());
            }
}

void BankCard::Deposit(Account acc) {
    // Implementation for depositing money into an account
    
}

void BankCard::Withdraw(Account acc) {
    // Implementation for withdrawing money from an account
    
}

void BankCard::Transfer(Account acc1, Account acc2) {
    // Implementation for transferring money between accounts on the card
}