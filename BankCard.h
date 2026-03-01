#pragma once
#include <string>
#include <vector>
#include "Account.h"

class BankCard {
public:
    BankCard(std::string userId, std::string pin, std::vector<Account> accounts);

    const std::string& UserId() const;
    bool CheckPin(const std::string& pin) const;

    std::vector<std::string> AccountNames() const;
    int AccountCount() const;

    double GetBalance(int idx) const;
    bool Deposit(int idx, double amount);
    bool Withdraw(int idx, double amount);
    bool Transfer(int fromIdx, int toIdx, double amount);

    bool ChangePin(const std::string& newPin);

private:
    std::string userId_;
    std::string pin_; // for class project; do NOT print it
    std::vector<Account> accounts_;

    bool ValidIndex(int idx) const;
};