/**********|**********|**********|
Program: main.cpp
Course: OOPDS
Trimester: 2420
Lecture Class:
Tutorial Class:
Trimester: 2430
Member_1: |  |  |
Member_2: 242UC244PP | Nicholas Beh Zhi Yang | NICHOLAS.BEH.ZHI@student.mmu.edu.my | 011-65215166
Member_3: 242UC24551 | LOW ZHENG HAO | LOW.ZHENG.HAO@student.mmu.edu.my | 013-8888444
**********|**********|**********/

#include <iostream>
#include <vector>
#include <string>
#include <limits> // for numeric_limits
#include <algorithm>

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

BankAccount createAccount()
{
    int accountNumber = getValidInt("Enter Account Number: ");
    int customerId = getValidInt("Enter Customer ID: ");

    string customerName;
    while (true)
    {
        cout << "Enter Customer Name: ";
        getline(cin, customerName);

        // check not empty and not only digits
        bool allDigits = !customerName.empty() &&
                         all_of(customerName.begin(), customerName.end(), ::isdigit);

        if (!customerName.empty() && !allDigits)
            break;
        cout << "Invalid name! Customer name cannot be empty or only numbers.\n";
    }

    string balInput;
    double balance = 0.0;
    while (true)
    {
        cout << "Enter Initial Balance: ";
        getline(cin, balInput);

        // check valid number (only digits)
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
        {
            balance = stod(balInput);
            break;
        }
        cout << "Invalid input. Please enter a valid number.\n";
    }

    if (balance < 0)
    {
        cout << "Balance cannot be negative. Setting to 0.\n";
        balance = 0;
    }

    return BankAccount(accountNumber, Customer(customerId, customerName), balance);
}

struct Node
{
    BankAccount account;
    Node *next;
};

void addAccount(Node *&head)
{
    BankAccount newAcc = createAccount();

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
    {
        head = iniAcc; // first node
    }
    else
    {
        Node *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = iniAcc; // attach at the end
    }

    cout << "Account added successfully!\n\n";
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