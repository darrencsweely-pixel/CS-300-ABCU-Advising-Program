# CS 300 ABCU Advising Assistance Program

A C++ command-line application developed for CS 300 that uses a binary search tree to organize, search, and display course information for ABC University (ABCU).

## Overview

The ABCU Advising Assistance Program is designed to help academic advisors quickly access course information. The program reads course data from a CSV file and stores each course in a binary search tree.

The application allows the user to:

- Load and validate course data from a CSV file
- Display all courses in alphanumeric order
- Search for an individual course
- Display a course title and its prerequisites
- Handle invalid menu selections, missing files, and invalid course numbers

## Data Structure

I chose a **binary search tree (BST)** for this project.

Each node in the tree stores a `Course` object containing:

- Course number
- Course title
- Prerequisite course numbers

Course numbers are used as the key when inserting and searching the tree. Courses with smaller course numbers are placed in the left subtree, while larger course numbers are placed in the right subtree.

One of the main reasons I chose a binary search tree was ABCU's requirement to display the entire course list in alphanumeric order. An in-order traversal of the tree naturally produces the courses from lowest to highest course number without requiring a separate sorting operation.

## Program Menu

The program provides the following menu:

```text
1. Load Data Structure
2. Print Course List
3. Print Course
9. Exit
```

Course data must be loaded before options 2 or 3 can be used.

## Input Validation

The program performs several checks while loading and using the course data.

It verifies that:

- The requested file can be opened
- Each course contains at least a course number and title
- Every listed prerequisite exists as another course in the input file
- Course data is loaded before attempting to print or search
- Menu selections are valid
- A requested course exists before displaying its information

Course-number searches are converted to uppercase, allowing input such as `csci300` to locate `CSCI300`.

## Example

Searching for `CSCI300` produces:

```text
CSCI300, Introduction to Algorithms
Prerequisites: CSCI200, Data Structures; MATH201, Discrete Mathematics
```

Printing the complete course list produces:

```text
CSCI100, Introduction to Computer Science
CSCI101, Introduction to Programming in C++
CSCI200, Data Structures
CSCI300, Introduction to Algorithms
CSCI301, Advanced Programming in C++
CSCI350, Operating Systems
CSCI400, Large Software Development
MATH201, Discrete Mathematics
```

## Build and Run

Compile the program using a C++17-compatible compiler:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic ProjectTwo.cpp -o ProjectTwo
```

Run the compiled program:

```bash
./ProjectTwo
```

When prompted to load the course data, enter:

```text
CS 300 ABCU_Advising_Program_Input.csv
```

## Testing

The program was tested for:

- Successful loading of the provided ABCU course data
- Correct alphanumeric ordering of all courses
- Courses with multiple prerequisites
- Courses with no prerequisites
- Lowercase course-number input
- Invalid course numbers
- Invalid menu selections
- Attempting to use the program before loading course data
- Missing input files

The final program successfully loaded all eight courses from the provided data file and correctly displayed the required course and prerequisite information.

## What I Learned

This project helped me connect the data structures covered in CS 300 to an actual software requirement. Comparing vectors, hash tables, and binary search trees showed me that choosing a data structure depends on how the data will actually be used rather than simply choosing the structure with the fastest operation in one category.

The most challenging part of the project was making sure the course data was valid before adding it to the tree. Since courses can have zero, one, or multiple prerequisites, I needed to verify that every prerequisite referred to another valid course. I also gained more experience working with tree traversal, searching, file input, input validation, and dynamically allocated memory in C++.

## Technologies

- C++
- Binary Search Trees
- File I/O
- CSV Parsing
- Data Validation
- Recursive Tree Traversal

## Author

**Darren Sweely**

CS 300 — Data Structures and Algorithms
