#pragma once
#include <string>
#include <vector>
#include "BankCard.h"

// BankSystem is the "core engine" storing users and accounts.
// The ATM UI calls into this class.
class BankSystem {
public:
    BankSystem();

    bool Authenticate(const std::string& userId, const std::string& pin);
    void Logout();

    bool IsLoggedIn() const;
    std::string CurrentUser() const;

    std::vector<std::string> GetUserAccounts() const;

    double GetBalance(int accountIndex) const;
    bool Deposit(int accountIndex, double amount);
    bool Withdraw(int accountIndex, double amount);
    bool Transfer(int fromIndex, int toIndex, double amount);

    bool ChangePin(const std::string& newPin);

private:
    std::vector<BankCard> cards_; // requirement: store data in vectors
    int loggedInIndex_ = -1;

    int FindUserIndex(const std::string& userId) const;
};