//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Harrison Labrecque
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    //If the root is not null, recursively delete the tree from the root.
    if (root != nullptr) {
        //delete the left and right children recursively.
        delete root->left;
        delete root->right;

        //delete the root node.
        delete root;
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    //call postOrder function to start the traversal from the root.
    this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
   //calls the preOrder function to start traversal from the root.
    this->preOrder(root);
}



/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    //Checks to see if the tree is empty.
    if (root == nullptr) {
        //If the tree is empty, create a new node with the provided bid.
        root = new Node(bid);
    }
    else {
        //If the tree is not empty, call the addNode to find the correct place for the bid
        //starting from the root.
        addNode(root, bid);
    }

}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // Removing a bid from the tree
    root = removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FIXME (8) Implement searching the tree for a bid
    // set current node equal to root
    Node* current = root;

    //Traverse the tree until the bidId is found or the end is reached.
    while (current != nullptr) {
        if (bidId == current->bid.bidId) {
            //If the current node's bidId matches the searched bidId, return the bid.
            return current->bid;
        }
        else if (bidId < current->bid.bidId) {
            //If the searched bidId is smaller, move to the left subtree.
            current = current->left;
        }
        else {
            //If the searched bidId is larger, move to the right subtree.
            current = current->right;
        }
    }
    //If no matching bidId was found, return a default bid.
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // If the new bid's bidId is smaller than the current node's bidId,
    // we need to go to the left subtree.
    if (bid.bidId < node->bid.bidId) {
        // If there is no left child, insert the new node here
        if (node->left == nullptr) {
            node->left = new Node(bid);  // Create a new node and attach it as the left child
        }
        else {
            // Otherwise, recurse down the left child
            addNode(node->left, bid);
        }
    }
    // If the new bid's bidId is larger than the current node's bidId,
    // we need to go to the right subtree.
    else if (bid.bidId > node->bid.bidId) {
        // If there is no right child, insert the new node here
        if (node->right == nullptr) {
            node->right = new Node(bid);  // Create a new node and attach it as the right child
        }
        else {
            // Otherwise, recurse down the right child
            addNode(node->right, bid);
        }
    }

}

void BinarySearchTree::inOrder(Node* node) {
    // Check if the node is not nullptr
    if (node != nullptr) {
        // Recurse to the left subtree first
        inOrder(node->left);

        // Output the bid details for the current node
        cout << "Bid ID: " << node->bid.bidId << " | "
            << "Title: " << node->bid.title << " | "
            << "Amount: " << node->bid.amount << " | "
            << "Fund: " << node->bid.fund << endl;

        // Recurse to the right subtree
        inOrder(node->right);
    }
}
void BinarySearchTree::postOrder(Node* node) {
    // Check if the node is not nullptr
    if (node != nullptr) {
        // Recurse to the left subtree
        postOrder(node->left);

        // Recurse to the right subtree
        postOrder(node->right);

        // Output the bid details for the current node
        cout << "Bid ID: " << node->bid.bidId << " | "
            << "Title: " << node->bid.title << " | "
            << "Amount: " << node->bid.amount << " | "
            << "Fund: " << node->bid.fund << endl;
    }

}

void BinarySearchTree::preOrder(Node* node) {
    // Check if the node is not nullptr
    if (node != nullptr) {
        // Output the bid details for the current node
        cout << "Bid ID: " << node->bid.bidId << " | "
            << "Title: " << node->bid.title << " | "
            << "Amount: " << node->bid.amount << " | "
            << "Fund: " << node->bid.fund << endl;

        // Recurse to the left subtree
        preOrder(node->left);

        // Recurse to the right subtree
        preOrder(node->right);
    }
}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // If the node is nullptr, return nullptr (base case for recursion)
    if (node == nullptr) {
        return node;
    }

    // If the bidId is smaller than the current node's bidId, go to the left subtree
    if (bidId < node->bid.bidId) {
        node->left = removeNode(node->left, bidId);
    }
    // If the bidId is greater than the current node's bidId, go to the right subtree
    else if (bidId > node->bid.bidId) {
        node->right = removeNode(node->right, bidId);
    }
    // If we found the node to be deleted
    else {
        // Case 1: Node has no children 
        if (node->left == nullptr && node->right == nullptr) {
            delete node; // Delete the node
            node = nullptr; // Set the pointer to nullptr
        }
        // Case 2: Node has one child 
        else if (node->left == nullptr) {
            Node* temp = node;
            node = node->right; // Move the node's right child up
            delete temp; // Delete the old node
        }
        else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left; // Move the node's left child up
            delete temp; // Delete the old node
        }
        // Case 3: Node has two children
        else {
            // Find the minimum node in the right subtree.
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left; // Move left until the minimum node is found
            }

            // Replace the node's data with the successor's data
            node->bid = temp->bid;

            // Now delete the successor (which is in the right subtree)
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }

    return node;
}



//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
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
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
