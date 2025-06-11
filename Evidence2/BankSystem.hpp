//
//  BankSystem.hpp
//  BankManagement
//
//  Created by Developer on 10/06/25.
//

#ifndef BankSystem_hpp
#define BankSystem_hpp

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <random>
#include <chrono>

enum class AccountType {
    CHECKING,
    SAVINGS,
    CREDIT,
    LOAN
};

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    PAYMENT,
    FEE,
    INTEREST
};

struct Date {
    int day, month, year;
    
    Date() : day(1), month(1), year(2025) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    bool isAfter(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
};

class Transaction {
private:
    static int nextTransactionId;
    int transactionId;
    TransactionType type;
    double amount;
    Date transactionDate;
    std::string description;
    int fromAccountId;
    int toAccountId;
    
public:
    Transaction(TransactionType transType, double amt, const std::string& desc, 
                int fromId = 0, int toId = 0)
        : transactionId(nextTransactionId++), type(transType), amount(amt),
          description(desc), fromAccountId(fromId), toAccountId(toId) {
        transactionDate = Date();
    }
    
    ~Transaction() {}
    
    // Getters
    int getId() const { return transactionId; }
    TransactionType getType() const { return type; }
    double getAmount() const { return amount; }
    Date getDate() const { return transactionDate; }
    std::string getDescription() const { return description; }
    int getFromAccountId() const { return fromAccountId; }
    int getToAccountId() const { return toAccountId; }
    
    std::string getTypeString() const {
        switch (type) {
            case TransactionType::DEPOSIT: return "Deposit";
            case TransactionType::WITHDRAWAL: return "Withdrawal";
            case TransactionType::TRANSFER: return "Transfer";
            case TransactionType::PAYMENT: return "Payment";
            case TransactionType::FEE: return "Fee";
            case TransactionType::INTEREST: return "Interest";
            default: return "Unknown";
        }
    }
    
    void printTransaction() const {
        std::cout << "[" << transactionId << "] " << getTypeString() 
                  << " - $" << std::fixed << std::setprecision(2) << amount
                  << " on " << transactionDate.toString() << std::endl;
        std::cout << "  Description: " << description << std::endl;
        if (fromAccountId > 0) {
            std::cout << "  From Account: " << fromAccountId << std::endl;
        }
        if (toAccountId > 0) {
            std::cout << "  To Account: " << toAccountId << std::endl;
        }
    }
};

int Transaction::nextTransactionId = 1;

class Account {
private:
    static int nextAccountId;
    int accountId;
    std::string accountNumber;
    AccountType type;
    double balance;
    double interestRate;
    double creditLimit;
    int customerId;
    Date openDate;
    bool isActive;
    std::vector<Transaction> transactionHistory;
    double monthlyFee;
    int freeTransactions;
    int transactionCount;
    
public:
    Account(AccountType accType, int custId, double initialBalance = 0.0)
        : accountId(nextAccountId++), type(accType), balance(initialBalance),
          interestRate(0.01), creditLimit(0.0), customerId(custId),
          isActive(true), monthlyFee(0.0), freeTransactions(5), transactionCount(0) {
        
        // Generate account number
        accountNumber = std::to_string(1000000000 + accountId);
        openDate = Date();
        
        // Set default values based on account type
        switch (type) {
            case AccountType::CHECKING:
                interestRate = 0.001;
                monthlyFee = 10.0;
                freeTransactions = 10;
                break;
            case AccountType::SAVINGS:
                interestRate = 0.02;
                monthlyFee = 5.0;
                freeTransactions = 5;
                break;
            case AccountType::CREDIT:
                interestRate = 0.18;
                creditLimit = 1000.0;
                monthlyFee = 0.0;
                break;
            case AccountType::LOAN:
                interestRate = 0.05;
                monthlyFee = 0.0;
                break;
        }
    }
    
    ~Account() {}
    
    bool deposit(double amount, const std::string& description = "Deposit") {
        if (amount <= 0 || !isActive) return false;
        
        balance += amount;
        Transaction trans(TransactionType::DEPOSIT, amount, description, 0, accountId);
        transactionHistory.push_back(trans);
        transactionCount++;
        
        return true;
    }
    
    bool withdraw(double amount, const std::string& description = "Withdrawal") {
        if (amount <= 0 || !isActive) return false;
        
        if (type == AccountType::CREDIT) {
            if (balance - amount < -creditLimit) return false;
        } else {
            if (balance < amount) return false;
        }
        
        balance -= amount;
        Transaction trans(TransactionType::WITHDRAWAL, amount, description, accountId, 0);
        transactionHistory.push_back(trans);
        transactionCount++;
        
        // Apply transaction fee if over limit
        if (transactionCount > freeTransactions) {
            double fee = 2.0;
            balance -= fee;
            Transaction feeTransaction(TransactionType::FEE, fee, "Transaction fee", accountId, 0);
            transactionHistory.push_back(feeTransaction);
        }
        
        return true;
    }
    
    bool transfer(double amount, Account* toAccount, const std::string& description = "Transfer") {
        if (!toAccount || amount <= 0 || !isActive || !toAccount->isActive) return false;
        
        if (withdraw(amount, "Transfer out - " + description)) {
            if (toAccount->deposit(amount, "Transfer in - " + description)) {
                // Update transaction records for transfer
                Transaction trans(TransactionType::TRANSFER, amount, description, accountId, toAccount->accountId);
                transactionHistory.push_back(trans);
                return true;
            } else {
                // Rollback if deposit fails
                deposit(amount, "Transfer rollback");
                return false;
            }
        }
        return false;
    }
    
    void applyMonthlyInterest() {
        if (!isActive) return;
        
        double interest = 0.0;
        if (type == AccountType::SAVINGS && balance > 0) {
            interest = balance * (interestRate / 12);
            balance += interest;
            Transaction trans(TransactionType::INTEREST, interest, "Monthly interest", 0, accountId);
            transactionHistory.push_back(trans);
        } else if (type == AccountType::CREDIT && balance < 0) {
            interest = abs(balance) * (interestRate / 12);
            balance -= interest;
            Transaction trans(TransactionType::INTEREST, interest, "Credit interest charge", accountId, 0);
            transactionHistory.push_back(trans);
        }
    }
    
    void applyMonthlyFee() {
        if (!isActive || monthlyFee <= 0) return;
        
        balance -= monthlyFee;
        Transaction trans(TransactionType::FEE, monthlyFee, "Monthly maintenance fee", accountId, 0);
        transactionHistory.push_back(trans);
    }
    
    void resetMonthlyCounters() {
        transactionCount = 0;
    }
    
    double getAvailableBalance() const {
        if (type == AccountType::CREDIT) {
            return creditLimit + balance;
        }
        return balance;
    }
    
    bool isOverdrawn() const {
        return balance < 0 && type != AccountType::CREDIT;
    }
    
    // Getters
    int getId() const { return accountId; }
    std::string getAccountNumber() const { return accountNumber; }
    AccountType getType() const { return type; }
    double getBalance() const { return balance; }
    double getInterestRate() const { return interestRate; }
    double getCreditLimit() const { return creditLimit; }
    int getCustomerId() const { return customerId; }
    Date getOpenDate() const { return openDate; }
    bool getIsActive() const { return isActive; }
    std::vector<Transaction> getTransactionHistory() const { return transactionHistory; }
    double getMonthlyFee() const { return monthlyFee; }
    int getFreeTransactions() const { return freeTransactions; }
    int getTransactionCount() const { return transactionCount; }
    
    // Setters
    void setInterestRate(double rate) { interestRate = rate; }
    void setCreditLimit(double limit) { creditLimit = limit; }
    void setIsActive(bool active) { isActive = active; }
    void setMonthlyFee(double fee) { monthlyFee = fee; }
    
    std::string getTypeString() const {
        switch (type) {
            case AccountType::CHECKING: return "Checking";
            case AccountType::SAVINGS: return "Savings";
            case AccountType::CREDIT: return "Credit";
            case AccountType::LOAN: return "Loan";
            default: return "Unknown";
        }
    }
    
    void printAccount() const {
        std::cout << "Account [" << accountId << "] " << accountNumber << std::endl;
        std::cout << "  Type: " << getTypeString() << std::endl;
        std::cout << "  Customer ID: " << customerId << std::endl;
        std::cout << "  Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        
        if (type == AccountType::CREDIT) {
            std::cout << "  Credit Limit: $" << std::fixed << std::setprecision(2) << creditLimit << std::endl;
            std::cout << "  Available Credit: $" << std::fixed << std::setprecision(2) << getAvailableBalance() << std::endl;
        }
        
        std::cout << "  Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
        std::cout << "  Monthly Fee: $" << std::fixed << std::setprecision(2) << monthlyFee << std::endl;
        std::cout << "  Opened: " << openDate.toString() << std::endl;
        std::cout << "  Status: " << (isActive ? "Active" : "Inactive") << std::endl;
        std::cout << "  Transactions this month: " << transactionCount << "/" << freeTransactions << " free" << std::endl;
        
        if (isOverdrawn()) {
            std::cout << "  ⚠️  ACCOUNT OVERDRAWN!" << std::endl;
        }
    }
    
    void printRecentTransactions(int count = 5) const {
        std::cout << "Recent Transactions (last " << count << "):" << std::endl;
        
        int start = std::max(0, static_cast<int>(transactionHistory.size()) - count);
        
        for (size_t i = start; i < transactionHistory.size(); ++i) {
            std::cout << "  ";
            transactionHistory[i].printTransaction();
        }
        
        if (transactionHistory.empty()) {
            std::cout << "  No transactions found." << std::endl;
        }
    }
};

int Account::nextAccountId = 1;

class Customer {
private:
    static int nextCustomerId;
    int customerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    Date birthDate;
    Date joinDate;
    bool isActive;
    std::vector<int> accountIds;
    std::string ssn;
    
public:
    Customer(const std::string& first, const std::string& last, 
             const std::string& emailAddr, const std::string& phoneNum)
        : customerId(nextCustomerId++), firstName(first), lastName(last),
          email(emailAddr), phone(phoneNum), isActive(true) {
        joinDate = Date();
        birthDate = Date(1, 1, 1990); // Default birth date
        ssn = "XXX-XX-" + std::to_string(1000 + customerId); // Masked SSN
    }
    
    ~Customer() {}
    
    void addAccount(int accountId) {
        if (std::find(accountIds.begin(), accountIds.end(), accountId) == accountIds.end()) {
            accountIds.push_back(accountId);
        }
    }
    
    void removeAccount(int accountId) {
        accountIds.erase(std::remove(accountIds.begin(), accountIds.end(), accountId), 
                        accountIds.end());
    }
    
    std::string getFullName() const {
        return firstName + " " + lastName;
    }
    
    int getAge() const {
        Date today;
        return today.year - birthDate.year;
    }
    
    // Getters
    int getId() const { return customerId; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    Date getBirthDate() const { return birthDate; }
    Date getJoinDate() const { return joinDate; }
    bool getIsActive() const { return isActive; }
    std::vector<int> getAccountIds() const { return accountIds; }
    std::string getSSN() const { return ssn; }
    
    // Setters
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setEmail(const std::string& emailAddr) { email = emailAddr; }
    void setPhone(const std::string& phoneNum) { phone = phoneNum; }
    void setAddress(const std::string& addr) { address = addr; }
    void setBirthDate(const Date& date) { birthDate = date; }
    void setIsActive(bool active) { isActive = active; }
    
    void printCustomer() const {
        std::cout << "Customer [" << customerId << "] " << getFullName() << std::endl;
        std::cout << "  Email: " << email << " | Phone: " << phone << std::endl;
        std::cout << "  Address: " << address << std::endl;
        std::cout << "  Age: " << getAge() << " | Joined: " << joinDate.toString() << std::endl;
        std::cout << "  SSN: " << ssn << std::endl;
        std::cout << "  Status: " << (isActive ? "Active" : "Inactive") << std::endl;
        std::cout << "  Accounts: " << accountIds.size() << std::endl;
    }
};

int Customer::nextCustomerId = 1;

class BankSystem {
private:
    std::vector<std::unique_ptr<Customer>> customers;
    std::vector<std::unique_ptr<Account>> accounts;
    std::string bankName;
    std::string branchCode;
    int totalCustomers;
    int totalAccounts;
    double totalDeposits;
    double totalLoans;
    
public:
    BankSystem(const std::string& name, const std::string& branch)
        : bankName(name), branchCode(branch), totalCustomers(0), 
          totalAccounts(0), totalDeposits(0.0), totalLoans(0.0) {}
    
    ~BankSystem() {}
    
    void addCustomer(std::unique_ptr<Customer> customer) {
        if (customer) {
            customers.push_back(std::move(customer));
            totalCustomers++;
        }
    }
    
    void addAccount(std::unique_ptr<Account> account) {
        if (account) {
            Customer* customer = findCustomer(account->getCustomerId());
            if (customer) {
                customer->addAccount(account->getId());
                accounts.push_back(std::move(account));
                totalAccounts++;
            }
        }
    }
    
    Customer* findCustomer(int customerId) {
        auto it = std::find_if(customers.begin(), customers.end(),
            [customerId](const std::unique_ptr<Customer>& customer) {
                return customer->getId() == customerId;
            });
        return (it != customers.end()) ? it->get() : nullptr;
    }
    
    Account* findAccount(int accountId) {
        auto it = std::find_if(accounts.begin(), accounts.end(),
            [accountId](const std::unique_ptr<Account>& account) {
                return account->getId() == accountId;
            });
        return (it != accounts.end()) ? it->get() : nullptr;
    }
    
    Account* findAccountByNumber(const std::string& accountNumber) {
        auto it = std::find_if(accounts.begin(), accounts.end(),
            [accountNumber](const std::unique_ptr<Account>& account) {
                return account->getAccountNumber() == accountNumber;
            });
        return (it != accounts.end()) ? it->get() : nullptr;
    }
    
    bool transferBetweenAccounts(int fromAccountId, int toAccountId, double amount) {
        Account* fromAccount = findAccount(fromAccountId);
        Account* toAccount = findAccount(toAccountId);
        
        if (fromAccount && toAccount) {
            return fromAccount->transfer(amount, toAccount, "Inter-account transfer");
        }
        return false;
    }
    
    void processMonthlyMaintenance() {
        std::cout << "Processing monthly maintenance..." << std::endl;
        
        for (const auto& account : accounts) {
            account->applyMonthlyInterest();
            account->applyMonthlyFee();
            account->resetMonthlyCounters();
        }
        
        updateBankStatistics();
        std::cout << "Monthly maintenance completed." << std::endl;
    }
    
    void updateBankStatistics() {
        totalDeposits = 0.0;
        totalLoans = 0.0;
        
        for (const auto& account : accounts) {
            if (account->getBalance() > 0) {
                totalDeposits += account->getBalance();
            } else {
                totalLoans += abs(account->getBalance());
            }
        }
    }
    
    std::vector<Customer*> getCustomersByStatus(bool active) {
        std::vector<Customer*> result;
        for (const auto& customer : customers) {
            if (customer->getIsActive() == active) {
                result.push_back(customer.get());
            }
        }
        return result;
    }
    
    std::vector<Account*> getAccountsByType(AccountType type) {
        std::vector<Account*> result;
        for (const auto& account : accounts) {
            if (account->getType() == type) {
                result.push_back(account.get());
            }
        }
        return result;
    }
    
    void printBankSummary() {
        updateBankStatistics();
        
        std::cout << "=== " << bankName << " - Branch " << branchCode << " ===" << std::endl;
        std::cout << "Total Customers: " << totalCustomers << std::endl;
        std::cout << "Total Accounts: " << totalAccounts << std::endl;
        std::cout << "Total Deposits: $" << std::fixed << std::setprecision(2) << totalDeposits << std::endl;
        std::cout << "Total Loans: $" << std::fixed << std::setprecision(2) << totalLoans << std::endl;
        
        // Account type breakdown
        std::cout << "\nAccount Breakdown:" << std::endl;
        std::cout << "  Checking: " << getAccountsByType(AccountType::CHECKING).size() << std::endl;
        std::cout << "  Savings: " << getAccountsByType(AccountType::SAVINGS).size() << std::endl;
        std::cout << "  Credit: " << getAccountsByType(AccountType::CREDIT).size() << std::endl;
        std::cout << "  Loan: " << getAccountsByType(AccountType::LOAN).size() << std::endl;
        
        // Customer status
        auto activeCustomers = getCustomersByStatus(true);
        auto inactiveCustomers = getCustomersByStatus(false);
        std::cout << "\nCustomer Status:" << std::endl;
        std::cout << "  Active: " << activeCustomers.size() << std::endl;
        std::cout << "  Inactive: " << inactiveCustomers.size() << std::endl;
    }
    
    void printAllCustomers() {
        std::cout << "=== All Customers ===" << std::endl;
        for (const auto& customer : customers) {
            customer->printCustomer();
            std::cout << std::endl;
        }
    }
    
    void printAllAccounts() {
        std::cout << "=== All Accounts ===" << std::endl;
        for (const auto& account : accounts) {
            account->printAccount();
            std::cout << std::endl;
        }
    }
};

int main() {
    std::cout << "=== Bank Management System ===" << std::endl;
    
    BankSystem bank("First National Bank", "MAIN001");
    
    // Create customers
    auto customer1 = std::make_unique<Customer>("John", "Smith", "john@email.com", "555-1234");
    auto customer2 = std::make_unique<Customer>("Jane", "Doe", "jane@email.com", "555-5678");
    auto customer3 = std::make_unique<Customer>("Bob", "Johnson", "bob@email.com", "555-9012");
    
    int custId1 = customer1->getId();
    int custId2 = customer2->getId();
    int custId3 = customer3->getId();
    
    bank.addCustomer(std::move(customer1));
    bank.addCustomer(std::move(customer2));
    bank.addCustomer(std::move(customer3));
    
    // Create accounts
    auto checking1 = std::make_unique<Account>(AccountType::CHECKING, custId1, 1000.0);
    auto savings1 = std::make_unique<Account>(AccountType::SAVINGS, custId1, 5000.0);
    auto checking2 = std::make_unique<Account>(AccountType::CHECKING, custId2, 2500.0);
    auto credit2 = std::make_unique<Account>(AccountType::CREDIT, custId2, 0.0);
    auto savings3 = std::make_unique<Account>(AccountType::SAVINGS, custId3, 10000.0);
    
    int checkingId1 = checking1->getId();
    int savingsId1 = savings1->getId();
    int checkingId2 = checking2->getId();
    int creditId2 = credit2->getId();
    
    bank.addAccount(std::move(checking1));
    bank.addAccount(std::move(savings1));
    bank.addAccount(std::move(checking2));
    bank.addAccount(std::move(credit2));
    bank.addAccount(std::move(savings3));
    
    // Perform transactions
    std::cout << "\n=== Performing Transactions ===" << std::endl;
    
    Account* johnChecking = bank.findAccount(checkingId1);
    Account* johnSavings = bank.findAccount(savingsId1);
    Account* janeChecking = bank.findAccount(checkingId2);
    Account* janeCredit = bank.findAccount(creditId2);
    
    if (johnChecking) {
        johnChecking->deposit(500.0, "Payroll deposit");
        johnChecking->withdraw(200.0, "ATM withdrawal");
        johnChecking->withdraw(50.0, "Coffee shop");
        johnChecking->withdraw(30.0, "Gas station");
    }
    
    if (johnSavings && johnChecking) {
        johnSavings->transfer(1000.0, johnChecking, "Emergency fund transfer");
    }
    
    if (janeChecking && janeCredit) {
        janeChecking->withdraw(300.0, "Grocery shopping");
        janeCredit->withdraw(500.0, "Credit purchase");
    }
    
    bank.transferBetweenAccounts(checkingId2, checkingId1, 250.0);
    
    // Print current status
    std::cout << "\n=== Current Bank Status ===" << std::endl;
    bank.printBankSummary();
    
    std::cout << "\n=== Account Details ===" << std::endl;
    if (johnChecking) {
        johnChecking->printAccount();
        std::cout << std::endl;
        johnChecking->printRecentTransactions();
        std::cout << std::endl;
    }
    
    if (janeCredit) {
        janeCredit->printAccount();
        std::cout << std::endl;
    }
    
    // Process monthly maintenance
    std::cout << "\n=== Monthly Maintenance ===" << std::endl;
    bank.processMonthlyMaintenance();
    
    std::cout << "\n=== Final Bank Summary ===" << std::endl;
    bank.printBankSummary();
    
    return 0;
}

#endif /* BankSystem_hpp */