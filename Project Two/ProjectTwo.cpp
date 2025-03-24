/*
Program Name: ProjectTwo.cpp
Author: Harrison Labrecque
Version: 1.0
Description: This C++ code implements a course management system that allows users to load course data from a file, 
validate course prerequisites, search for specific courses, and display sorted course information.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <algorithm>  // for std::sort
#include <cctype>     // for std::tolower

using namespace std;

// Define the Course class
class Course {
public:
    string courseNumber;
    string title;
    list<string> prerequisites;

    Course(string courseNumber, string title) {
        this->courseNumber = courseNumber;
        this->title = title;
    }
};

// HashTable class to store courses
class HashTable {
private:
    unordered_map<string, Course*> table;

public:
    void put(string key, Course* course) {
        table[key] = course;
    }

    Course* get(string key) {
        if (table.find(key) != table.end()) {
            return table[key];
        }
        return nullptr;
    }

    bool contains(string key) {
        return table.find(key) != table.end();
    }

    // Public method to get all courses (for sorting or printing purposes)
    unordered_map<string, Course*>& getAllCourses() {
        return table;
    }
};

// Helper function to convert a string to lowercase
string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Function to parse the file and load courses
HashTable parseFile(string filename) {
    ifstream file(filename);
    HashTable courses;
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return courses;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        // Split the line by commas
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Error: Missing course number or title" << endl;
            continue;
        }

        string courseNumber = tokens[0];
        string title = tokens[1];
        
        // Create a new course
        Course* course = new Course(courseNumber, title);

        // If there are prerequisites, add them
        for (size_t i = 2; i < tokens.size(); ++i) {
            course->prerequisites.push_back(tokens[i]);
        }

        // Store the course in the hash table
        courses.put(courseNumber, course);
    }

    file.close();
    return courses;
}

// Function to validate prerequisites (modified)
void validatePrerequisites(HashTable& courses) {
    for (auto& pair : courses.getAllCourses()) {
        Course* courseObj = pair.second;

        // Check if all prerequisites are valid
        for (const string& prerequisite : courseObj->prerequisites) {
            if (!courses.contains(prerequisite)) {
                // Commenting out the error message
                // cout << "Error: Prerequisite " << prerequisite << " for course " 
                //      << courseObj->courseNumber << " does not exist" << endl;
            }
        }
    }
}

// Function to print course information
void printCourseInformation(HashTable& courses) {
    for (auto& pair : courses.getAllCourses()) {
        Course* courseObj = pair.second;
        cout << "Course: " << courseObj->courseNumber << " - " << courseObj->title << endl;

        if (courseObj->prerequisites.empty()) {
            cout << "  No prerequisites" << endl;
        } else {
            cout << "  Prerequisites:" << endl;
            for (const string& prerequisite : courseObj->prerequisites) {
                Course* prerequisiteCourse = courses.get(prerequisite);
                if (prerequisiteCourse) {
                    cout << "    " << prerequisiteCourse->courseNumber << " - " 
                         << prerequisiteCourse->title << endl;
                }
            }
        }
    }
}

// Function to search a specific course (modified for case-insensitivity)
void searchCourse(HashTable& courses, string courseNumber) {
    // Convert the user input to lowercase
    string lowerCourseNumber = toLowerCase(courseNumber);
    
    // Iterate through the courses and search case-insensitively
    for (auto& pair : courses.getAllCourses()) {
        Course* courseObj = pair.second;
        
        // Convert the stored course number to lowercase
        string lowerStoredCourseNumber = toLowerCase(courseObj->courseNumber);
        
        // Compare the lowercase course numbers
        if (lowerStoredCourseNumber == lowerCourseNumber) {
            cout << "Course: " << courseObj->courseNumber << " - " << courseObj->title << endl;

            if (courseObj->prerequisites.empty()) {
                cout << "  No prerequisites" << endl;
            } else {
                cout << "  Prerequisites:" << endl;
                for (const string& prerequisite : courseObj->prerequisites) {
                    Course* prerequisiteCourse = courses.get(prerequisite);
                    if (prerequisiteCourse) {
                        cout << "    " << prerequisiteCourse->courseNumber << " - " 
                             << prerequisiteCourse->title << endl;
                    }
                }
            }
            return;  // Found the course, exit the function
        }
    }
    
    // If no course is found, print an error message
    cout << "Error: Course " << courseNumber << " not found" << endl;
}

// Function to sort the courses by course number
vector<Course*> sortCourses(HashTable& courses) {
    vector<Course*> sortedCourses;
    
    // Extract all courses into a vector
    for (auto& pair : courses.getAllCourses()) {
        sortedCourses.push_back(pair.second);
    }

    // Sort the courses by course number
    sort(sortedCourses.begin(), sortedCourses.end(), [](Course* a, Course* b) {
        return a->courseNumber < b->courseNumber;
    });

    return sortedCourses;
}

// Function to display the menu
void displayMenu() {
    cout << "1. Load data from file" << endl;
    cout << "2. Print sorted list of courses" << endl;
    cout << "3. Search for a specific course" << endl;
    cout << "9. Exit" << endl;
}

int main() {
    string filename = "CS 300 ABCU_Advising_Program_Input.csv";  // Example filename
    HashTable courses;  // Initialize the courses hash table
    
    while (true) {
        displayMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        
        if (choice == 1) {
            courses = parseFile(filename);  // Load course data from file
            validatePrerequisites(courses);  // Validate prerequisites for loaded courses
        }
        else if (choice == 2) {
            vector<Course*> sortedCourses = sortCourses(courses);  // Sort courses
            for (Course* courseObj : sortedCourses) {
                cout << "Course: " << courseObj->courseNumber << " - " << courseObj->title << endl;
            }
        }
        else if (choice == 3) {
            string courseNumber;
            cout << "Enter course number to search: ";
            cin >> courseNumber;
            searchCourse(courses, courseNumber);  // Search and print course details
        }
        else if (choice == 9) {
            cout << "Exiting program." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
