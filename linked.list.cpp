#include <iostream>

using namespace std;

struct Node
{
    int data;
    Node *next;
};

int main()
{
    Node *head = new Node();
    Node *second = new Node();
    Node *third = new Node();

    head->data = 10;
    second->data = 20;
    third->data = 30;

    head->next = second;
    second->next = third;
    third->next = nullptr;

    Node *temp = head; // start from head
    while (temp != nullptr)
    {
        cout << temp->data << " ";
        temp = temp->next; // move to the next node
    }

    // Free memory (good practice)
    delete head;
    delete second;
    delete third;

    return 0;
}