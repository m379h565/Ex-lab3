#include <iostream>
#include <iomanip>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for " << getType() << " (ID: " << getAccountNumber() << "):" << std::endl;
        std::cout << "   Holder: " << getAccountHolder() << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    virtual std::string getType() const {
        return "Account";
    }

    double getBalance() const {
        return balance;
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }

    std::string getAccountHolder() const {
        return accountHolder;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            std::cout << "Insufficient funds for withdrawal." << std::endl;
        }
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    std::string getType() const override {
        return "Savings Account";
    }

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100; // Minimum balance required for savings account
        if (balance - amount >= minBalance) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal failed. Minimum balance requirement not met." << std::endl;
        }
    }

    double getInterestRate() const {
        return interestRate;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    std::string getType() const override {
        return "Current Account";
    }

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal failed. Exceeds overdraft limit." << std::endl;
        }
    }

    double getOverdraftLimit() const {
        return overdraftLimit;
    }

    // Operator overloading for transferring money between accounts
    CurrentAccount operator+(const SavingsAccount& savingsAcc) const {
        CurrentAccount result = *this;  // Create a copy of the current account
        result.deposit(savingsAcc.getBalance());
        const_cast<SavingsAccount&>(savingsAcc).withdraw(savingsAcc.getBalance());
        return result;
    }
};

// Operator overloading for displaying account details
std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "Account Details for " << account.getType() << " (ID: " << account.getAccountNumber() << "):" << std::endl;
    os << "   Holder: " << account.getAccountHolder() << std::endl;
    os << "   Balance: $" << std::fixed << std::setprecision(2) << account.getBalance() << std::endl;

    // Check if the account is of type Current Account to display overdraft limit
    if (const auto* currentAcc = dynamic_cast<const CurrentAccount*>(&account)) {
        os << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << currentAcc->getOverdraftLimit() << std::endl;
    }

    // Check if the account is of type Savings Account to display interest rate
    if (const auto* savingsAcc = dynamic_cast<const SavingsAccount*>(&account)) {
        os << "   Interest Rate: " << std::fixed << std::setprecision(2) << savingsAcc->getInterestRate() * 100 << "%" << std::endl;
    }

    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    // Transfer 300 from savings to current
    current = current + savings;  // Assuming the entire balance is transferred

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    return 0;
}
