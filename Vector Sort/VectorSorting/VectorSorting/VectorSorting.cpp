//============================================================================
// Name        : VectorSorting.cpp
// Author      : Harrison Labrecque
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Vector Sorting Algorithms
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
 * Prompt user for bid information using console (std::in)
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
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    // set low and high equal to the first and last indices.
    int low = begin;        // Starting point of the low index
    int high = end;         // Starting point of the high index

    // picking the pivot point (the middle element of the range).
    int pivot = begin + (end - begin) / 2;

    // Flag to indicate when the partition is complete and exits the loop.
    bool done = false;

    while (!done) {
        //Increment the low index as long as the title is less than the pivot title.
        while (bids.at(low).title.compare(bids.at(pivot).title) < 0) {
            ++low;
        }

        //Decrement the high index as long as the title at the high is greater than the pivot title.
        while (bids.at(pivot).title.compare(bids.at(high).title) < 0) {
            --high;
        }

        //If the low pointer crosses or equals the high pointer, the partition is complete.
        if (low >= high) {
            done = true;
        }
        else {
            //Swap the elements at the low and the high pointers.
            swap(bids.at(low), bids.at(high));

            //Move the pointers toward each other (low to the right, high to the left)
            ++low;
            --high;
        }
    }
    // returns the index where the pivot should be placed
    return high;
    }


    



/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    // Initialize 'mid' to hold the pivot index after partitioning.
    int mid = 0;  

    // Base case: If the array has 1 or fewer elements, it's already sorted.
    if (begin >= end) {  
        return;
    }
    // Partition the array and get the pivot index.
    mid = partition(bids, begin, end);  

     // Recursively sort the left partition (elements smaller than the pivot).
    quickSort(bids, begin, mid); 
    // Recursively sort the right partition (elements larger than the pivot).
    quickSort(bids, mid + 1, end);  
}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    // Define min as the index of the current minimum bid
    int min = 0;

    // Check size of bids vector
    size_t size = bids.size();

    // pos is the position within bids that divides sorted/unsorted
    // Loop through all elements except the last one (sorted portion grows from left to right)
    for (size_t pos = 0; pos < size - 1; ++pos) {
        // Set min = pos (assume the current element is the smallest in the unsorted part)
        min = pos;

        // Loop over the remaining elements to the right of position pos
        for (size_t j = pos + 1; j < size; ++j) {
            // If this element's title is less than the current minimum title
            if (bids[j].title < bids[min].title) {
                // This element becomes the new minimum
                min = j;
            }
        }

        // Swap the current minimum with the smaller one found
        if (min != pos) {
            swap(bids[min], bids[pos]);  // Swap using the built-in vector method
        }
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
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        // FIXME (1b): Invoke the selection sort and report timing results
        case 3:
            //Initialize a timer variable before loading the bids.
            ticks = clock();

            //Call the SelectionSort method to load the bids;
            selectionSort(bids);

            //Displays how the size of bids to the screen.
            cout << bids.size() << " bids sorted" << endl;

            //Calculate the elapsed time and displays the results to the screen.
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;
        
         // FIXME (2b): Invoke the quick sort and report timing results
        case 4:
            // Start a timer variable before loading the bids.
            ticks = clock();

            //Call the QuickSort method to load the bids.
            quickSort(bids, 0, bids.size() - 1);

            //Displays the size of bids to the screen.
            cout << bids.size() << " bids sorted" << endl;

            //Calculate the elapsed time and display the results to the screen.
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;
        

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
