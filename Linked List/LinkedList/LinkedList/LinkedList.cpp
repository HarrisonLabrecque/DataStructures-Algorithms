//============================================================================
// Name        : LinkedList.cpp
// Author      : Harrison Labrecque
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // FIXME (1): Initialize housekeeping variables
    //set head and tail equal to nullptr
    head = nullptr;
    tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* tempNode = nullptr;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        tempNode = current; // hang on to current node
        current = current->next; // make current the next node
        delete tempNode; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    //Creating a new node
    Node* nextNode = new Node(bid);

    if (head == nullptr) {
        //if the list is empty, both the head and tail would point to the next node.
        head = nextNode;
        tail = nextNode;
    }
    else {
        //if the list is not empty, append to the end of the list.
        tail->next = nextNode; // current tail node points to the new node
        tail = nextNode; // update the tail to the new node.
    }
    
    //increment the size of the linked list.
    size++;
 
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    //creating a new node.
    Node* nextNode = new Node(bid);

    //Checks to see if the list is not empty.
    if (head != nullptr) {
        //if the list is not empty, make the new node's next pointer point to the current head of the list.
        nextNode->next = head;
        
    }
    // set the new node as the head of the list.
    head = nextNode;

    //increment the size of the list by one since a new node was added to the front of the list.
    size++;

}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    //start with the head of the list.
    Node* currentNode = head;

    //traverse the entire list,as long as there are nodes.
    while (currentNode != nullptr) {
        //prints the bid information.
        cout << currentNode->bid.bidId << ": "
            << currentNode->bid.title << " | "
            << currentNode->bid.amount << " | "
            << currentNode->bid.fund << endl;
        
        //move to the next node in the list.
        currentNode = currentNode->next;
    }
  
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // declare Node* variables
    Node* currentNode = nullptr;
    Node* tempNode = nullptr;

    if (head == nullptr) {
        //if the list is empty then it returns nothing.
        return;
    }
    //special case: if the head node has to be removed
    if (head->bid.bidId == bidId) {
        //Saves the current head node, moves the head pointer to the next node, and removes the old head node.
        //The size of the list is decreased before the loop is broken.
        tempNode = head;
        head = head->next;
        delete tempNode;
        size--;

        return;
    }

    //General case to remove a node from the linked list if bidId matches.
    currentNode = head;
    while (currentNode->next != nullptr) {
        // if the next node has the matching bidId.
        if (currentNode->next->bid.bidId == bidId) {
            tempNode = currentNode->next; // holding the current node to remove
            currentNode->next = tempNode->next; //move current node beyond the next node.
            delete tempNode; // frees up memory 
            size--; // decreases the size of the linked list.

            return; //exits the loop.

        }
        //move to the next node.
        currentNode = currentNode->next;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    //Special case: if the matching bid is the head.
    if (head != nullptr && head->bid.bidId == bidId) {
        //returns the bid from the head.
        return head->bid;
    }

    //Start at the head of the list.
    Node* currentNode = head;

    //loop through the list until the end is reached.
    while (currentNode != nullptr) {
        // if the current node's bidId matches the given bidId, return the corresponding bid.
        if (currentNode->bid.bidId == bidId) {
            return currentNode->bid;
        }
        //move to the next node.
        currentNode = currentNode->next;
    }
    //if no match was found, it would return an empty bid.
    return Bid();
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            bidList.Remove(bidKey);

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
