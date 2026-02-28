#include <iostream>
#include <string>
#include <vector>
#include <limits>

// ---------- Interfaces ----------
class IAuthService {
public:
    virtual ~IAuthService() = default;
    virtual bool Authenticate(const std::string& id, const std::string& pin) = 0;
};

class IAccountService {
public:
    virtual ~IAccountService() = default;

    virtual std::vector<std::string> GetUserAccounts(const std::string& userId) = 0;
    virtual double GetBalance(const std::string& userId, int accountIndex) = 0;

    virtual bool Deposit(const std::string& userId, int accountIndex, double amount) = 0;
    virtual bool Withdraw(const std::string& userId, int accountIndex, double amount) = 0;
    virtual bool Transfer(const std::string& userId, int fromIndex, int toIndex, double amount) = 0;

    // Optional security
    virtual bool ChangePin(const std::string& userId, const std::string& newPin) = 0;
};

// ---------- Simple input helpers  ----------
static void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int ReadIntInRange(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            ClearCin();
            if (value >= minVal && value <= maxVal) return value;
        } else {
            ClearCin();
        }
        std::cout << "Invalid input. Enter a number from " << minVal << " to " << maxVal << ".\n";
    }
}

static double ReadPositiveAmount(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double amt;
        if (std::cin >> amt) {
            ClearCin();
            if (amt > 0.0) return amt;
        } else {
            ClearCin();
        }
        std::cout << "Invalid amount. Enter a positive number.\n";
    }
}

// NOTE: PIN masking with * is OS/terminal-specific.
static std::string ReadLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

// ---------- ATM UI / Control Flow ----------
class ATM {
public:
    ATM(IAuthService& auth, IAccountService& accounts)
        : auth_(auth), accounts_(accounts) {}

    void Run() {
        bool running = true;
        while (running) {
            PrintWelcome();

            if (!LoginFlow()) {
                // If login fails, allow retry or exit
                int choice = ReadIntInRange("1) Try again\n2) Exit\nSelect: ", 1, 2);
                if (choice == 2) running = false;
                continue;
            }

            // Authenticated menu loop
            bool inSession = true;
            while (inSession) {
                ShowMainMenu();
                int choice = ReadIntInRange("Select an option: ", 1, 6);
                inSession = HandleMenuChoice(choice);
            }

            Logout();
        }

        std::cout << "Goodbye.\n";
    }

private:
    IAuthService& auth_;
    IAccountService& accounts_;
    std::string currentUserId_;
    bool authenticated_ = false;

    void PrintWelcome() {
        std::cout << "\n==============================\n";
        std::cout << "      Welcome to C++ GP ATM\n";
        std::cout << "==============================\n";
    }

    bool LoginFlow() {
        currentUserId_.clear();
        authenticated_ = false;

        std::string id = ReadLine("Enter User ID: ");
        std::string pin = ReadLine("Enter PIN: "); // Replace later with masked input

        if (auth_.Authenticate(id, pin)) {
            currentUserId_ = id;
            authenticated_ = true;
            std::cout << "Login successful.\n\n";
            return true;
        }

        std::cout << "Login failed: Invalid ID or PIN.\n\n";
        return false;
    }

    void ShowMainMenu() {
        std::cout << "--------- Main Menu ----------\n";
        std::cout << "1) View Balance\n";
        std::cout << "2) Deposit\n";
        std::cout << "3) Withdraw\n";
        std::cout << "4) Transfer\n";
        std::cout << "5) Change PIN\n";
        std::cout << "6) Logout\n";
        std::cout << "------------------------------\n";
    }

    bool HandleMenuChoice(int choice) {
        if (!authenticated_) {
            std::cout << "Session expired. Please log in again.\n";
            return false;
        }

        switch (choice) {
        case 1: ViewBalance(); return true;
        case 2: DoDeposit();   return true;
        case 3: DoWithdraw();  return true;
        case 4: DoTransfer();  return true;
        case 5: DoChangePin(); return true;
        case 6: return false; // logout
        default:
            std::cout << "Invalid menu choice.\n";
            return true;
        }
    }

    int SelectAccount(const std::string& title) {
        std::vector<std::string> accs = accounts_.GetUserAccounts(currentUserId_);
        if (accs.empty()) {
            std::cout << "No accounts found for this user.\n";
            return -1;
        }

        std::cout << title << "\n";
        for (int i = 0; i < static_cast<int>(accs.size()); i++) {
            std::cout << (i + 1) << ") " << accs[i] << "\n";
        }
        int selection = ReadIntInRange("Choose account: ", 1, static_cast<int>(accs.size()));
        return selection - 1; // 0-based index
    }

    void ViewBalance() {
        int idx = SelectAccount("Select an account to view balance:");
        if (idx < 0) return;

        double bal = accounts_.GetBalance(currentUserId_, idx);
        std::cout << "Current Balance: $" << bal << "\n\n";
    }

    void DoDeposit() {
        int idx = SelectAccount("Select an account to deposit into:");
        if (idx < 0) return;

        double amt = ReadPositiveAmount("Enter deposit amount: $");
        if (accounts_.Deposit(currentUserId_, idx, amt)) {
            std::cout << "Deposit successful.\n\n";
        } else {
            std::cout << "Deposit failed.\n\n";
        }
    }

    void DoWithdraw() {
        int idx = SelectAccount("Select an account to withdraw from:");
        if (idx < 0) return;

        double amt = ReadPositiveAmount("Enter withdrawal amount: $");
        if (accounts_.Withdraw(currentUserId_, idx, amt)) {
            std::cout << "Withdrawal successful.\n\n";
        } else {
            std::cout << "Withdrawal failed (insufficient funds or invalid).\n\n";
        }
    }

    void DoTransfer() {
        int fromIdx = SelectAccount("Select the FROM account:");
        if (fromIdx < 0) return;

        int toIdx = SelectAccount("Select the TO account:");
        if (toIdx < 0) return;

        if (fromIdx == toIdx) {
            std::cout << "Transfer canceled: accounts must be different.\n\n";
            return;
        }

        double amt = ReadPositiveAmount("Enter transfer amount: $");
        if (accounts_.Transfer(currentUserId_, fromIdx, toIdx, amt)) {
            std::cout << "Transfer successful.\n\n";
        } else {
            std::cout << "Transfer failed (insufficient funds or invalid).\n\n";
        }
    }

    void DoChangePin() {
        std::string newPin = ReadLine("Enter new PIN: ");
        if (accounts_.ChangePin(currentUserId_, newPin)) {
            std::cout << "PIN updated.\n\n";
        } else {
            std::cout << "PIN change failed.\n\n";
        }
    }

    void Logout() {
        authenticated_ = false;
        currentUserId_.clear();
        std::cout << "Logged out.\n";
    }
};

// ---------- Dummy services for testing ----------
class DummyAuth : public IAuthService {
public:
    bool Authenticate(const std::string& id, const std::string& pin) override {
        // Example: two users
        if (id == "user1" && pin == "1111") return true;
        if (id == "user2" && pin == "2222") return true;
        return false;
    }
};

class DummyAccounts : public IAccountService {
public:
    std::vector<std::string> GetUserAccounts(const std::string& userId) override {
        if (userId == "user1") return { "Checking", "Savings" };
        if (userId == "user2") return { "Checking" };
        return {};
    }

    double GetBalance(const std::string& userId, int accountIndex) override {
        return 1000.00; // stub
    }

    bool Deposit(const std::string&, int, double) override { return true; }
    bool Withdraw(const std::string&, int, double) override { return true; }
    bool Transfer(const std::string&, int, int, double) override { return true; }
    bool ChangePin(const std::string&, const std::string&) override { return true; }
};

int main() {
    DummyAuth auth;
    DummyAccounts accounts;
    ATM atm(auth, accounts);
    atm.Run();
    return 0;
}
