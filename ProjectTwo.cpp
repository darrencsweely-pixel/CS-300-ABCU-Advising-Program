//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Darren Sweely
// Course      : CS 300
// Description : ABCU Advising Assistance Program
//
// AI Use Acknowledgment:
// I used ChatGPT to help organize, review, and test this program against the
// project requirements. I reviewed the final code and its logic.
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Stores the information for one ABCU course.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node used by the binary search tree.
struct Node {
    Course course;
    Node* left;
    Node* right;

    explicit Node(const Course& aCourse)
        : course(aCourse), left(nullptr), right(nullptr) {
    }
};

class BinarySearchTree {
private:
    Node* root;

    // Recursively inserts a course based on its course number.
    Node* insertNode(Node* node, const Course& course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        } else {
            // Replace duplicate course data instead of creating a duplicate node.
            node->course = course;
        }

        return node;
    }

    // In-order traversal naturally prints the BST in alphanumeric order.
    void printInOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        printInOrder(node->left);
        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << endl;
        printInOrder(node->right);
    }

    // Recursively frees all dynamically allocated tree nodes.
    void deleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {
    }

    ~BinarySearchTree() {
        clear();
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
    }

    bool empty() const {
        return root == nullptr;
    }

    void insert(const Course& course) {
        root = insertNode(root, course);
    }

    // Iteratively searches the tree for a course number.
    const Course* search(const string& courseNumber) const {
        Node* current = root;

        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return nullptr;
    }

    void printAll() const {
        printInOrder(root);
    }
};

// Removes leading and trailing whitespace from a string.
string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Converts course numbers to uppercase so searches are user-friendly.
string toUpper(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return value;
}

// Splits one CSV line into fields. The supplied ABCU file does not contain
// quoted commas, so a simple comma-delimited parser is appropriate here.
vector<string> splitCsvLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream stream(line);

    while (getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }

    // Preserve a final empty CSV field when a line ends with a comma.
    if (!line.empty() && line.back() == ',') {
        fields.push_back("");
    }

    return fields;
}

// Checks that each prerequisite refers to another course in the input file.
bool validatePrerequisites(const vector<Course>& courses) {
    for (const Course& course : courses) {
        for (const string& prerequisite : course.prerequisites) {
            bool found = false;

            for (const Course& possibleCourse : courses) {
                if (possibleCourse.courseNumber == prerequisite) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                cerr << "Error: prerequisite " << prerequisite
                     << " for " << course.courseNumber
                     << " does not exist in the course file." << endl;
                return false;
            }
        }
    }

    return true;
}

// Reads, validates, and loads the ABCU course file into the BST.
bool loadCourses(const string& fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Error: unable to open file \"" << fileName << "\"." << endl;
        return false;
    }

    vector<Course> courses;
    string line;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitCsvLine(line);

        // Every valid line must contain a course number and title.
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
            cerr << "Error: invalid course data on line "
                 << lineNumber << "." << endl;
            return false;
        }

        Course course;
        course.courseNumber = toUpper(fields[0]);
        course.courseTitle = fields[1];

        // Any nonempty fields after the title are prerequisite course numbers.
        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpper(fields[i]));
            }
        }

        courses.push_back(course);
    }

    inputFile.close();

    if (courses.empty()) {
        cerr << "Error: the course file contains no course data." << endl;
        return false;
    }

    if (!validatePrerequisites(courses)) {
        return false;
    }

    // Only replace the existing tree after the entire file passes validation.
    courseTree.clear();

    for (const Course& course : courses) {
        courseTree.insert(course);
    }

    cout << courses.size() << " courses loaded successfully." << endl;
    return true;
}

// Displays one course and the full titles of its prerequisites.
void printCourseInformation(const BinarySearchTree& courseTree,
                            const string& courseNumber) {
    const Course* course = courseTree.search(toUpper(trim(courseNumber)));

    if (course == nullptr) {
        cout << "Course " << toUpper(trim(courseNumber))
             << " not found." << endl;
        return;
    }

    cout << course->courseNumber << ", "
         << course->courseTitle << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None" << endl;
        return;
    }

    for (size_t i = 0; i < course->prerequisites.size(); ++i) {
        const string& prerequisiteNumber = course->prerequisites[i];
        const Course* prerequisite = courseTree.search(prerequisiteNumber);

        if (i > 0) {
            cout << "; ";
        }

        if (prerequisite != nullptr) {
            cout << prerequisite->courseNumber << ", "
                 << prerequisite->courseTitle;
        } else {
            // This should not occur after successful file validation.
            cout << prerequisiteNumber;
        }
    }

    cout << endl;
}

void printMenu() {
    cout << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    string input;

    cout << "Welcome to the ABCU course planner." << endl;

    while (true) {
        printMenu();
        getline(cin, input);

        int choice;

        try {
            size_t position;
            choice = stoi(trim(input), &position);

            if (position != trim(input).length()) {
                throw invalid_argument("extra characters");
            }
        } catch (...) {
            cout << "Invalid menu option. Please enter 1, 2, 3, or 9." << endl;
            continue;
        }

        if (choice == 1) {
            string fileName;

            cout << "What is the name of the course data file? ";
            getline(cin, fileName);

            dataLoaded = loadCourses(trim(fileName), courseTree);
        } else if (choice == 2) {
            if (!dataLoaded || courseTree.empty()) {
                cout << "Please load the course data first using option 1." << endl;
                continue;
            }

            cout << endl;
            cout << "Here is a sample schedule:" << endl;
            courseTree.printAll();
        } else if (choice == 3) {
            if (!dataLoaded || courseTree.empty()) {
                cout << "Please load the course data first using option 1." << endl;
                continue;
            }

            string courseNumber;
            cout << "What course do you want to know about? ";
            getline(cin, courseNumber);

            printCourseInformation(courseTree, courseNumber);
        } else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        } else {
            cout << choice << " is not a valid option." << endl;
            cout << "Please enter 1, 2, 3, or 9." << endl;
        }
    }

    return 0;
}
