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
#include <string>

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

BankAccount createAccount()
{
    int accountNumber;
    string customerName;
    double balance;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    cin.ignore();
    cout << "Enter Customer Name: ";
    getline(cin, customerName);

    cout << "Enter Initial Balance: ";
    cin >> balance;

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

int main()
{
    int choice;
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
        cout << " Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            addAccount(head);
            cout << "Account created successfully!\n";
            break;

        case 2:
            displayAllAccounts(head);
            break;
        }
    }

    return 0;
}