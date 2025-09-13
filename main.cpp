/**********|**********|**********|
Program: main.cpp
Course: OOPDS
Trimester: 2420
Lecture Class:
Tutorial Class:
Trimester: 2430
Member_1: 243UC247BZ | LimJianFeng | Lim.Jian.Feng@student.mmu.edu.my | 019-9002733
Member_2: 242UC244PP | Nicholas Beh Zhi Yang | NICHOLAS.BEH.ZHI@student.mmu.edu.my | 011-65215166
Member_3: 242UC24551 | LOW ZHENG HAO | LOW.ZHENG.HAO@student.mmu.edu.my | 013-8888444
**********|**********|**********/

#include <iostream>
#include <vector>
#include <string>
#include <limits> // for numeric_limits
#include <algorithm>
#include <fstream> // file handling

using namespace std;

class Customer
{
private:
    int customerId;
    string name;

public:
    Customer(int id = 0, string n = " ") : customerId(id), name(n) {}

    int getId() const { return customerId; }
    string getName() const { return name; }

    void display() const
    {
        cout << "Customer #" << customerId << " | Name: " << name;
    }
};

class BankAccount
{
private:
    int accountNumber;
    Customer customer;
    double balance;

public:
    BankAccount(int accNo = 0, Customer c = Customer(), double bal = 0.0) : accountNumber(accNo), customer(c), balance(bal) {}

    int getAccountNo() const { return accountNumber; }
    Customer getCustomer() const { return customer; }
    double getBalance() const { return balance; }

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited: " << amount << " | New Balance: " << balance << endl
                 << endl;
        }
        else
        {
            cout << "Invalid deposit amount!\n";
        }
    }

    bool withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid withdrawal amount!\n\n";
            return false;
        }
        if (amount > balance)
        {
            cout << "Insufficient balance!\n\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrawn: " << amount << " | New Balance: " << balance << endl;
        cout << endl;
        return true;
    }

    void display() const
    {
        cout << "Account #" << accountNumber << " | ";
        customer.display();
        cout << " | Balance: " << balance << endl
             << endl;
    }
};

int getValidInt(const string &prompt)
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // check all characters are digits
        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit))
        {
            return stoi(input);
        }
        cout << "Invalid input. Please enter a number.\n";
    }
}

void saveAccountToFile(const BankAccount &acc)
{
    ofstream outFile("accounts.txt", ios::app); // append mode
    if (!outFile)
    {
        cout << "Error opening file for writing!\n";
        return;
    }

    outFile << acc.getAccountNo() << ","
            << acc.getCustomer().getId() << ","
            << acc.getCustomer().getName() << ","
            << acc.getBalance() << "\n";

    outFile.close();
}

BankAccount createAccount(bool &cancelled)
{
    string accInput, cusIdInput, customerName, balInput;
    int accountNumber = 0, customerId = 0;
    double balance = 0.0;
    cancelled = false;

    while (true)
    {
        vector<string> errors;

        cout << "\n--- Create New Account ---\n";
        cout << "Enter Account Number: ";
        getline(cin, accInput);

        cout << "Enter Customer ID: ";
        getline(cin, cusIdInput);

        cout << "Enter Customer Name: ";
        getline(cin, customerName);

        cout << "Enter Initial Balance: ";
        getline(cin, balInput);

        // Validate Account Number
        if (!accInput.empty() && all_of(accInput.begin(), accInput.end(), ::isdigit))
            accountNumber = stoi(accInput);
        else
            errors.push_back("Account Number must be a valid number.");

        // Validate Customer ID
        if (!cusIdInput.empty() && all_of(cusIdInput.begin(), cusIdInput.end(), ::isdigit))
            customerId = stoi(cusIdInput);
        else
            errors.push_back("Customer ID must be a valid number.");

        // Validate Customer Name
        bool allDigits = !customerName.empty() &&
                         all_of(customerName.begin(), customerName.end(), ::isdigit);
        if (customerName.empty() || allDigits)
            errors.push_back("Customer Name cannot be empty or only numbers.");

        // Validate Balance
        bool ok = true, dotSeen = false;
        for (char c : balInput)
        {
            if (isdigit(c))
                continue;
            if (c == '.' && !dotSeen)
            {
                dotSeen = true;
                continue;
            }
            ok = false;
            break;
        }
        if (ok && !balInput.empty())
            balance = stod(balInput);
        else
            errors.push_back("Initial Balance must be a valid number.");
        if (balance < 0)
        {
            errors.push_back("Initial Balance cannot be negative.");
            balance = 0;
        }

        // If no errors → return new account
        if (errors.empty())
        {
            return BankAccount(accountNumber, Customer(customerId, customerName), balance);
        }

        // Show all error messages at once
        cout << "\nErrors:\n";
        for (const auto &err : errors)
            cout << "- " << err << "\n";

        // Ask user if they want to retry or quit to main menu
        cout << "\nDo you want to try again? (y/n): ";
        string choice;
        getline(cin, choice);
        if (choice == "n" || choice == "N")
        {
            cancelled = true;
            return BankAccount(); // return empty account
        }
    }
}

struct Node
{
    BankAccount account;
    Node *next;
};

void addAccount(Node *&head)
{
    bool cancelled = false;
    BankAccount newAcc = createAccount(cancelled);

    if (cancelled)
    {
        cout << "Returning to main menu...\n\n";
        return;
    }

    // Check for duplicates
    Node *current = head;
    while (current != nullptr)
    {
        if (current->account.getAccountNo() == newAcc.getAccountNo())
        {
            cout << "Error: Account number already exists!\n\n";
            return;
        }
        if (current->account.getCustomer().getName() == newAcc.getCustomer().getName())
        {
            cout << "Error: Customer name already exists!\n\n";
            return;
        }
        current = current->next;
    }

    Node *iniAcc = new Node;
    iniAcc->account = newAcc;
    iniAcc->next = nullptr;

    if (head == nullptr)
        head = iniAcc;
    else
    {
        Node *temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = iniAcc;
    }

    saveAccountToFile(newAcc); // save to file
    cout << "Account added successfully and saved to file!\n\n";
}

void updateFile(Node *head)
{
    ofstream outFile("accounts.txt");
    if (!outFile)
    {
        cout << "Error opening file for writing!\n";
        return;
    }
    Node *current = head;
    while (current != nullptr)
    {
        outFile << current->account.getAccountNo() << ","
                << current->account.getCustomer().getId() << ","
                << current->account.getCustomer().getName() << ","
                << current->account.getBalance() << "\n";
        current = current->next;
    }
    outFile.close();
}

void loadAccountsFromFile(Node *&head)
{
    ifstream inFile("accounts.txt");
    if (!inFile)
    {
        // file may not exist on first run, just return
        return;
    }

    int accNo, cusId;
    string name;
    double balance;

    string line;
    while (getline(inFile, line))
    {
        if (line.empty())
            continue;

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        accNo = stoi(line.substr(0, pos1));
        cusId = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        name = line.substr(pos2 + 1, pos3 - pos2 - 1);
        balance = stod(line.substr(pos3 + 1));

        BankAccount acc(accNo, Customer(cusId, name), balance);

        // insert into linked list
        Node *newNode = new Node{acc, nullptr};
        if (head == nullptr)
            head = newNode;
        else
        {
            Node *temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = newNode;
        }
    }

    inFile.close();
}

void displayAllAccounts(Node *head)
{
    if (head == nullptr)
    {
        cout << "No accounts to display.\n\n";
        return;
    }

    Node *temp = head;
    while (temp != nullptr)
    {
        temp->account.display(); // use BankAccount display()
        temp = temp->next;
    }
}

// Search by Account Number (supports partial match too)
void searchAccNo(Node *head, int accNo)
{
    Node *current = head;
    bool found = false;

    string query = to_string(accNo); // convert to string for partial match

    while (current != nullptr)
    {
        string idStr = to_string(current->account.getAccountNo());
        if (idStr.find(query) != string::npos) // partial match
        {
            current->account.display();
            found = true;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "No account matches account number: " << accNo << endl;
    }
}

// Search by Customer Name (supports partial match)
void searchCusName(Node *head, const string &name)
{
    Node *current = head;
    bool found = false;

    while (current != nullptr)
    {
        if (current->account.getCustomer().getName().find(name) != string::npos)
        {
            current->account.display();
            found = true;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "No account matches customer name: " << name << endl;
    }
}

void depositMoney(Node *head)
{
    if (head == nullptr)
    {
        cout << "No accounts available!\n";
        return;
    }

    int accNum = getValidInt("Enter Account Number: ");
    Node *current = head;
    bool found = false;

    while (current != nullptr)
    {
        if (current->account.getAccountNo() == accNum)
        {
            double amount;
            cout << "Enter amount to deposit: ";
            while (!(cin >> amount))
            {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            current->account.deposit(amount);
            updateFile(head);
            found = true;
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "Account not found!\n\n";
    }
}

void withdrawMoney(Node *head)
{
    if (head == nullptr)
    {
        cout << "No accounts available!\n";
        return;
    }

    int accNum = getValidInt("Enter Account Number: ");
    Node *current = head;
    bool found = false;

    while (current != nullptr)
    {
        if (current->account.getAccountNo() == accNum)
        {
            double amount;
            cout << "Enter amount to withdraw: ";
            while (!(cin >> amount))
            {
                cout << "Invalid input. Please enter a number.\n\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (amount > current->account.getBalance())
            {
                cout << "Insufficient balance!\n\n";
            }
            else
            {
                current->account.withdraw(amount);
                updateFile(head);
            }
            found = true;
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "Account not found!\n";
    }
}

void deleteAccount(Node *&head)
{
    if (head == nullptr)
    {
        cout << "No accounts to delete!\n\n";
        return;
    }

    int accNum = getValidInt("Enter Account Number to delete: ");
    Node *current = head;
    Node *prev = nullptr;

    while (current != nullptr)
    {
        if (current->account.getAccountNo() == accNum)
        {
            if (prev == nullptr)
            {
                // deleting the head
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            delete current;
            cout << "Account deleted successfully!\n\n";
            return;
        }
        prev = current;
        current = current->next;
    }

    cout << "Account not found!\n";
}

// Free all allocated memory before exiting for cleaner program
void freeAllAccounts(Node *&head)
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}

int main()
{
    Node *head = nullptr; // start with empty list

    // Load saved accounts into memory at startup
    loadAccountsFromFile(head);

    while (true)
    {
        cout << " ****************************** " << endl;
        cout << " ----MENU----" << endl;
        cout << " 1. Add a new account " << endl;
        cout << " 2. Display all accounts " << endl;
        cout << " 3. Deposit money " << endl;
        cout << " 4. Withdraw money " << endl;
        cout << " 5. Search account " << endl;
        cout << " 6. Delete account " << endl;
        cout << " 7. Exit " << endl;
        cout << endl;
        cout << " ****************************** " << endl;
        cout << endl;

        int choice = getValidInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            addAccount(head);
            break;

        case 2:
            displayAllAccounts(head);
            break;

        case 3:
            depositMoney(head);
            break;

        case 4:
            withdrawMoney(head);
            break;

        case 5:
        {
            // Search through all accounts to find the matching one
            int searchChoice = getValidInt("Search by:\n1. Account Number\n2. Customer Name\nEnter your choice: ");

            if (searchChoice == 1)
            {
                int accNo = getValidInt("Enter account number: ");
                searchAccNo(head, accNo);
            }
            else if (searchChoice == 2)
            {
                string name;
                cout << "Enter customer name: ";
                getline(cin, name);
                searchCusName(head, name);
            }
            else
            {
                cout << "Invalid search choice.\n\n";
            }
            break;
        }

        case 6:
            deleteAccount(head);
            break;

        case 7:
            cout << "Exiting program...\n\n";
            freeAllAccounts(head); // clean up before exiting
            return 0;

        default:
            cout << "Invalid choice. Try again.\n\n";
        }
    }

    return 0;
}