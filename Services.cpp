#include "Services.h"

BankSystem::BankSystem() {
    // Two users; one has two accounts (assignment requirement)
    cards_.push_back(BankCard(
        "cppuser1", "1234",
        {
            Account(AccountType::Checking, "1000001234", 1200.00),
            Account(AccountType::Savings,  "2000009876", 5000.00)
        }
    ));

    cards_.push_back(BankCard(
        "gpclient2", "5678",
        {
            Account(AccountType::Checking, "1000005555", 800.00)
        }
    ));
}

int BankSystem::FindUserIndex(const std::string& userId) const {
    for (int i = 0; i < static_cast<int>(cards_.size()); ++i) {
        if (cards_[i].UserId() == userId) return i;
    }
    return -1;
}

bool BankSystem::Authenticate(const std::string& userId, const std::string& pin) {
    int idx = FindUserIndex(userId);
    if (idx < 0) return false;
    if (!cards_[idx].CheckPin(pin)) return false;
    loggedInIndex_ = idx;
    return true;
}

void BankSystem::Logout() { loggedInIndex_ = -1; }

bool BankSystem::IsLoggedIn() const { return loggedInIndex_ >= 0; }

std::string BankSystem::CurrentUser() const {
    if (!IsLoggedIn()) return {};
    return cards_[loggedInIndex_].UserId();
}

std::vector<std::string> BankSystem::GetUserAccounts() const {
    if (!IsLoggedIn()) return {};
    return cards_[loggedInIndex_].AccountNames();
}

double BankSystem::GetBalance(int accountIndex) const {
    if (!IsLoggedIn()) return 0.0;
    return cards_[loggedInIndex_].GetBalance(accountIndex);
}

bool BankSystem::Deposit(int accountIndex, double amount) {
    if (!IsLoggedIn()) return false;
    return cards_[loggedInIndex_].Deposit(accountIndex, amount);
}

bool BankSystem::Withdraw(int accountIndex, double amount) {
    if (!IsLoggedIn()) return false;
    return cards_[loggedInIndex_].Withdraw(accountIndex, amount);
}

bool BankSystem::Transfer(int fromIndex, int toIndex, double amount) {
    if (!IsLoggedIn()) return false;
    return cards_[loggedInIndex_].Transfer(fromIndex, toIndex, amount);
}

bool BankSystem::ChangePin(const std::string& newPin) {
    if (!IsLoggedIn()) return false;
    return cards_[loggedInIndex_].ChangePin(newPin);
}