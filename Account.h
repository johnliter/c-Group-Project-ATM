#pragma once
#include <string>

enum class AccountType { Checking, Savings };

class Account {
public:
    Account(AccountType type, std::string number, double startingBalance);

    AccountType Type() const;
    const std::string& Number() const;
    std::string DisplayName() const;

    double Balance() const;
    bool Deposit(double amount);
    bool Withdraw(double amount);

private:
    AccountType type_;
    std::string number_;
    double balance_;
};