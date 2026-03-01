#include "BankCard.h"
#include <cctype>
#include <algorithm>

BankCard::BankCard(std::string userId, std::string pin, std::vector<Account> accounts)
    : userId_(std::move(userId)), pin_(std::move(pin)), accounts_(std::move(accounts)) {}

const std::string& BankCard::UserId() const { return userId_; }

bool BankCard::CheckPin(const std::string& pin) const {
    return pin == pin_;
}

std::vector<std::string> BankCard::AccountNames() const {
    std::vector<std::string> out;
    out.reserve(accounts_.size());
    for (const auto& a : accounts_) out.push_back(a.DisplayName());
    return out;
}

int BankCard::AccountCount() const { return static_cast<int>(accounts_.size()); }

bool BankCard::ValidIndex(int idx) const {
    return idx >= 0 && idx < static_cast<int>(accounts_.size());
}

double BankCard::GetBalance(int idx) const {
    if (!ValidIndex(idx)) return 0.0;
    return accounts_[idx].Balance();
}

bool BankCard::Deposit(int idx, double amount) {
    if (!ValidIndex(idx)) return false;
    return accounts_[idx].Deposit(amount);
}

bool BankCard::Withdraw(int idx, double amount) {
    if (!ValidIndex(idx)) return false;
    return accounts_[idx].Withdraw(amount);
}

bool BankCard::Transfer(int fromIdx, int toIdx, double amount) {
    if (!ValidIndex(fromIdx) || !ValidIndex(toIdx)) return false;
    if (fromIdx == toIdx) return false;
    if (amount <= 0.0) return false;

    // Withdraw first, then deposit (simple transactional pattern)
    if (!accounts_[fromIdx].Withdraw(amount)) return false;
    accounts_[toIdx].Deposit(amount);
    return true;
}

bool BankCard::ChangePin(const std::string& newPin) {
    // simple policy: 4 digits
    if (newPin.size() != 4) return false;
    if (!std::all_of(newPin.begin(), newPin.end(), ::isdigit)) return false;
    pin_ = newPin;
    return true;
}