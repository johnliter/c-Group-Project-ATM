#include "Account.h"
#include <sstream>
#include <iomanip>

Account::Account(AccountType type, std::string number, double startingBalance)
    : type_(type), number_(std::move(number)), balance_(startingBalance) {}

AccountType Account::Type() const { return type_; }
const std::string& Account::Number() const { return number_; }

std::string Account::DisplayName() const {
    // Show only last 4 digits for privacy
    std::string last4 = (number_.size() >= 4) ? number_.substr(number_.size() - 4) : number_;
    std::ostringstream oss;
    oss << (type_ == AccountType::Checking ? "Checking" : "Savings") << " ••••" << last4;
    return oss.str();
}

double Account::Balance() const { return balance_; }

bool Account::Deposit(double amount) {
    if (amount <= 0.0) return false;
    balance_ += amount;
    return true;
}

bool Account::Withdraw(double amount) {
    if (amount <= 0.0) return false;
    if (amount > balance_) return false;
    balance_ -= amount;
    return true;
}