# Timetable-Generator
This repository hosts the source code and documentation for an innovative software solution designed for academic scheduling in higher education institutions. Built on robust graph theory principles and advanced graph coloring algorithms, our generator is a sophisticated tool for creating optimized and equitable class schedules.

Key Features:

Graph-Based Approach: Utilizes graph theory to model relationships and dependencies between courses, ensuring efficient use of resources.
Intelligent Graph Coloring: Seamlessly assigns courses to distinct time slots, preventing time conflicts and optimizing resource utilization.
User-Friendly Interface: Designed with simplicity and practicality in mind, allowing easy customization of the scheduling process.
Adherence to Constraints: Enforces constraints such as preventing consecutive classes of the same subject and limiting the number of classes per day.
Room Assignment Optimization: Efficiently assigns available rooms to courses, enhancing overall resource utilization.
# How to Use:

Input File: Provide a structured file with essential information about courses, their nomenclature, and shared students.
Customization: Tailor the scheduling process by specifying the number and names of available rooms.
Run the Generator: Run the checking.cpp file and let the algorithm work its magic, creating visually intuitive and human-readable timetables.

# Input File format
4
Math Chemistry Biology English
0
1
Alice
2
Bob
Charlie
3
David
Eve
Frank
2
Grace
Hank
0

# Explaination
Number of Courses (4): There are four courses in total.

Course Names:

Math
Chemistry
Biology
English
Common Students:

Math and Chemistry have 0 common students.
Chemistry and Biology have 1 common student, Alice.
Biology and English have 2 common students, Bob and Charlie.
English and Math have 3 common students, David, Eve, and Frank.
Biology and English have 2 common students, Grace and Hank.

The last 0 denotes a delimiter .


