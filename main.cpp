#include <iostream>
#include <string>
#include <limits>
#include "Services.h"

static void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int ReadIntInRange(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            ClearCin();
            if (v >= minVal && v <= maxVal) return v;
        } else {
            ClearCin();
        }
        std::cout << "Invalid input. Enter " << minVal << " to " << maxVal << ".\n";
    }
}

static double ReadPositiveAmount(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double a;
        if (std::cin >> a) {
            ClearCin();
            if (a > 0.0) return a;
        } else {
            ClearCin();
        }
        std::cout << "Invalid amount. Enter a positive number.\n";
    }
}

static std::string ReadLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static int SelectAccount(const BankSystem& bank, const std::string& title) {
    auto accs = bank.GetUserAccounts();
    if (accs.empty()) {
        std::cout << "No accounts available.\n";
        return -1;
    }
    std::cout << title << "\n";
    for (int i = 0; i < (int)accs.size(); ++i) {
        std::cout << (i + 1) << ") " << accs[i] << "\n";
    }
    int pick = ReadIntInRange("Choose account: ", 1, (int)accs.size());
    return pick - 1;
}

int main() {
    BankSystem bank;

    std::cout << "\n==============================\n";
    std::cout << "      Welcome to C++ GP ATM\n";
    std::cout << "==============================\n";

    // Simple security measure: limit login attempts (privacy/abuse control)
    const int maxAttempts = 3;

    while (true) {
        int attempts = 0;
        while (attempts < maxAttempts && !bank.IsLoggedIn()) {
            std::string id = ReadLine("Enter User ID: ");
            std::string pin = ReadLine("Enter PIN: ");

            if (bank.Authenticate(id, pin)) {
                std::cout << "Login successful.\n\n";
                break;
            }

            attempts++;
            std::cout << "Login failed. Attempts remaining: " << (maxAttempts - attempts) << "\n\n";
        }

        if (!bank.IsLoggedIn()) {
            std::cout << "Too many failed attempts. Exiting.\n";
            return 0;
        }

        bool session = true;
        while (session) {
            std::cout << "--------- Main Menu ----------\n";
            std::cout << "1) View Balance\n";
            std::cout << "2) Deposit\n";
            std::cout << "3) Withdraw\n";
            std::cout << "4) Transfer\n";
            std::cout << "5) Change PIN\n";
            std::cout << "6) Logout\n";
            std::cout << "------------------------------\n";

            int choice = ReadIntInRange("Select an option: ", 1, 6);

            switch (choice) {
                case 1: {
                    int idx = SelectAccount(bank, "Select an account to view balance:");
                    if (idx >= 0) std::cout << "Balance: $" << bank.GetBalance(idx) << "\n\n";
                    break;
                }
                case 2: {
                    int idx = SelectAccount(bank, "Select an account to deposit into:");
                    if (idx >= 0) {
                        double amt = ReadPositiveAmount("Deposit amount: $");
                        std::cout << (bank.Deposit(idx, amt) ? "Deposit successful.\n\n" : "Deposit failed.\n\n");
                    }
                    break;
                }
                case 3: {
                    int idx = SelectAccount(bank, "Select an account to withdraw from:");
                    if (idx >= 0) {
                        double amt = ReadPositiveAmount("Withdrawal amount: $");
                        std::cout << (bank.Withdraw(idx, amt) ? "Withdrawal successful.\n\n" : "Withdrawal failed (insufficient funds).\n\n");
                    }
                    break;
                }
                case 4: {
                    int from = SelectAccount(bank, "Select FROM account:");
                    if (from < 0) break;
                    int to = SelectAccount(bank, "Select TO account:");
                    if (to < 0) break;
                    if (from == to) { std::cout << "Transfer canceled: choose different accounts.\n\n"; break; }
                    double amt = ReadPositiveAmount("Transfer amount: $");
                    std::cout << (bank.Transfer(from, to, amt) ? "Transfer successful.\n\n" : "Transfer failed.\n\n");
                    break;
                }
                case 5: {
                    std::string newPin = ReadLine("Enter new 4-digit PIN: ");
                    std::cout << (bank.ChangePin(newPin) ? "PIN updated.\n\n" : "PIN change failed.\n\n");
                    break;
                }
                case 6:
                    bank.Logout();
                    std::cout << "Logged out.\n\n";
                    session = false;
                    break;
            }
        }
    }
}