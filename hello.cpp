#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;


class Account {
protected:
    string accountNumber;
    string accountHolder;
    double balance;

public:
    // Constructor
    Account(const string& accNum, const string& holder, double initialBalance)
        : accountNumber(accNum), accountHolder(holder), balance(initialBalance) {
        if (initialBalance < 0.0) {
            cout << "Warning: Initial balance cannot be negative. Setting to 0." << endl;
            balance = 0.0;
        }
    }

    virtual ~Account() {
        cout << "Destroying account: " << accountNumber << endl;
    }

    double getBalance() const {
        return balance;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolder() const {
        return accountHolder;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << amount << ". New balance: $" << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

   
    virtual bool withdraw(double amount) = 0;

    
    virtual void displayDetails() const = 0;
};


class SavingAccount : public Account {
private:
    double interestRate;

public:
    
    SavingAccount(const string& accNum, const string& holder, double initialBalance, double rate)
        : Account(accNum, holder, initialBalance), interestRate(rate) {}

    bool withdraw(double amount) override {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            cout << "Withdrawal: $" << amount << ". New balance: $" << balance << endl;
            return true;
        } else if (balance < amount) {
            cout << "Insufficient funds. Balance: $" << balance << endl;
            return false;
        } else {
            cout << "Invalid withdrawal amount." << endl;
            return false;
        }
    }

   
    void displayDetails() const override {
        cout << "--- Savings Account ---" << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: $" << balance << endl;
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "------------------------" << endl;
    }

    
    void applyInterest() {
        double interest = balance * interestRate;
        cout << "Applying interest: $" << interest << endl;
        deposit(interest); // Use the base class's deposit method
    }
};


class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(const string& accNum, const string& holder, double initialBalance, double limit)
        : Account(accNum, holder, initialBalance), overdraftLimit(limit) {}

   
    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return false;
        }
        
        // Check if withdrawal is possible within balance + overdraft
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
            cout << "Withdrawal: $" << amount << ". New balance: $" << balance << endl;
            if (balance < 0) {
                cout << "Warning: You are in overdraft!" << endl;
            }
            return true;
        } else {
            cout << "Insufficient funds, even with overdraft. Max available: $" << (balance + overdraftLimit) << endl;
            return false;
        }
    }

    
    void displayDetails() const override {
        cout << "--- Checking Account ---" << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: $" << balance << endl;
        cout << "Overdraft Limit: $" << overdraftLimit << endl;
        cout << "------------------------" << endl;
    }
};


class Bank {
private:
    
    vector<unique_ptr<Account>> accounts;

public:
    // Create and add a new savings account
    void openSavingAccount(const string& holder, double initialBalance, double rate) {
        string accNum = "S" + to_string(accounts.size() + 1001);
        accounts.push_back(make_unique<SavingAccount>(accNum, holder, initialBalance, rate));
        cout << "Opened new Savings Account with number: " << accNum << endl;
    }

    // Create and add a new checking account
    void openCheckingAccount(const string& holder, double initialBalance, double limit) {
        string accNum = "C" + to_string(accounts.size() + 2001);
        accounts.push_back(make_unique<CheckingAccount>(accNum, holder, initialBalance, limit));
        cout << "Opened new Checking Account with number: " << accNum << endl;
    }

    // Find an account by its number
    Account* findAccount(const string& accNum) {
        for (const auto& acc : accounts) {
            if (acc->getAccountNumber() == accNum) {
                return acc.get(); // Return a raw pointer for temporary use
            }
        }
        return nullptr; // Not found
    }

    // Display details for all accounts
    void displayAllAccounts() {
        cout << "\n--- All Bank Accounts ---" << endl;
        for (const auto& acc : accounts) {
            acc->displayDetails();
        }
    }

    
    ~Bank() {
        cout << "\nBank is closing. All accounts are being destroyed." << endl;
    }
};

int main() {
    
    Bank myBank;

    cout << "--- Opening Accounts ---" << endl;
    myBank.openSavingAccount("Alice Smith", 1000.0, 0.03);
    myBank.openCheckingAccount("Bob Johnson", 500.0, 100.0);
    cout << endl;

    cout << "--- Transactions for Alice (S1001) ---" << endl;
    Account* aliceAcc = myBank.findAccount("S1001");
    if (aliceAcc) {
        aliceAcc->deposit(250.0);
        aliceAcc->withdraw(100.0);
        aliceAcc->withdraw(1500.0);
        
       
        SavingAccount* aliceSavingAcc = dynamic_cast<SavingAccount*>(aliceAcc);
        if (aliceSavingAcc) {
            aliceSavingAcc->applyInterest();
        }
    }
    cout << endl;

    cout << "--- Transactions for Bob (C2002) ---" << endl;
    Account* bobAcc = myBank.findAccount("C2002");
    if (bobAcc) {
        bobAcc->withdraw(400.0);
        bobAcc->withdraw(150.0); 
        bobAcc->withdraw(100.0); 
        bobAcc->deposit(50.0); 
    }
    cout << endl;

    
    myBank.displayAllAccounts();
    
  
    
    return 0;
}
