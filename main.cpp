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

using namespace std;

class BankAccount
{
private:
    int accountNumber;
    string customerName;
    double balance;

public:
    BankAccount(int accNo = 0, string name = " ", double bal = 0.0)
    {
        accountNumber = accNo;
        customerName = name;
        balance = bal;
    }

    int getAccountNo() const { return accountNumber; }
    string getCustomerName() const { return customerName; }
    double getBalance() const { return balance; }

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited: " << amount << " | New Balance: " << balance << endl;
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
            cout << "Invalid withdrawal amount!\n";
            return false;
        }
        if (amount > balance)
        {
            cout << "Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrawn: " << amount << " | New Balance: " << balance << endl;
        return true;
    }

    void display() const
    {
        cout << "Account #" << accountNumber << " | Name: " << customerName << " | Balance: " << balance << endl;
    }
};

int getValidInt(const string &prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value) // valid int
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
            return value;
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();                                         // reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
        }
    }
}

BankAccount createAccount()
{
    int accountNumber = getValidInt("Enter Account Number: ");

    string customerName;
    cout << "Enter Customer Name: ";
    getline(cin, customerName);

    double balance;
    cout << "Enter Initial Balance: ";
    while (!(cin >> balance))
    {
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (balance < 0)
    {
        cout << "Balance cannot be negative. Setting to 0.\n";
        balance = 0;
    }

    return BankAccount(accountNumber, customerName, balance);
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
            cout << "Error: Account number already exists!\n";
            return;
        }
        if (current->account.getCustomerName() == newAcc.getCustomerName())
        {
            cout << "Error: Customer name already exists!\n";
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

    cout << "Account added successfully!\n";
    cout << endl;
}

void displayAllAccounts(Node *head)
{
    if (head == nullptr)
    {
        cout << "No accounts to display.\n";
        return;
    }

    Node *temp = head;
    while (temp != nullptr)
    {
        temp->account.display(); // use your BankAccount display()
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
        if (current->account.getCustomerName().find(name) != string::npos) // substring match
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

        case 5:
        {
            int searchChoice = getValidInt("Search by:\n1. Account Number\n2. Customer Name\nEnter your choice: ");

            if (searchChoice == 1)
            {
                int accNo = getValidInt("Enter account number: ");
                searchAccNo(head, accNo);
            }
            else if (searchChoice == 2)
            {
                string name;
                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter customer name: ";
                getline(cin, name);
                searchCusName(head, name);
            }
            else
            {
                cout << "Invalid search choice.\n";
            }
            break;
        }

        case 7:
            cout << "Exiting program...\n";
            return 0;

        default:
            cout << "Invalid choice. Try again.\n";
            cout << endl;
        }
    }

    return 0;
}