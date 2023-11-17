#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <map>
#include <vector>

using namespace std;

// Structure to represent a class schedule entry with room number
struct ScheduleEntry {
    string course;
    string time;
    int roomNumber;
};

// Function to perform graph coloring using backtracking
void colorGraph(const unordered_map<string, set<string > > &graph, map<string, int> &coloring, const string &node, int &chromaticNumber) {
    set<int> usedColors;

    // Check colors of adjacent nodes
    for (const string &neighbor : graph.at(node)) {
        if (coloring.find(neighbor) != coloring.end()) {
            usedColors.insert(coloring[neighbor]);
        }
    }

    // Assign the smallest available color
    for (int color = 1;; ++color) {
        if (usedColors.find(color) == usedColors.end()) {
            coloring[node] = color;
            chromaticNumber = max(chromaticNumber, color);
            break;
        }
    }

    // Recursively color the adjacent nodes
    for (const string &neighbor : graph.at(node)) {
        if (coloring.find(neighbor) == coloring.end()) {
            colorGraph(graph, coloring, neighbor, chromaticNumber);
        }
    }
}

// Function to display the schedule in a tabular form with auto-assigned room numbers
void displayScheduleWithRooms(const unordered_map<string, set<string> > &graph, const map<string, int> &coloring,
                              int chromaticNumber, const vector<int> &availableRooms) {
    cout << "\nNumber of time slots needed: " << chromaticNumber << endl;

    // Organize courses by color (time slot)
    map<int, vector<string> > coursesByColor;
    for (const auto &entry : coloring) {
        coursesByColor[entry.second].push_back(entry.first);
    }

    // Map room numbers to their corresponding time slots
    map<int, vector<ScheduleEntry> > scheduleWithRooms;

    int currentDay = 1; // 1 represents Monday
    int currentHour = 8;
    int currentMinute = 30;

    // Map course to the number of times it has been scheduled on a particular day
    unordered_map<string, int> coursesScheduledPerDay;

    for (int color = 1; color <= chromaticNumber; ++color) {
        for (const string &course : coursesByColor[color]) {
            int classesPerWeek;
            cout << "Enter the number of classes per week for " << course << ": ";
            cin >> classesPerWeek;

            for (int i = 0; i < classesPerWeek; ++i) {
                // Adjust the schedule if it's the lunch break
                if (currentHour == 12 && currentMinute >= 35 && currentMinute < 90) {
                    currentHour = 13;
                    currentMinute = 30;
                }

                int roomIndex = (i % availableRooms.size());
                int roomNumber = availableRooms[roomIndex];

                ScheduleEntry entry;
                entry.course = course;
                entry.time = to_string(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + to_string(currentMinute);
                entry.roomNumber = roomNumber;

                // Check the number of times the course has been scheduled on the current day
                if (coursesScheduledPerDay[course] < 2) {
                    scheduleWithRooms[currentDay].push_back(entry);
                    coursesScheduledPerDay[course]++;
                } else {
                    // Move to the next day if more than two classes are scheduled for the current day
                    currentDay = (currentDay % 5) + 1;
                    currentHour = 8;
                    currentMinute = 30;
                    coursesScheduledPerDay.clear(); // Reset the count for the new day
                    --i; // Revisit the current class on the new day
                    continue;
                }

                // Move to the next time slot
                currentMinute += 60;
                if (currentMinute >= 60) {
                    currentHour += 1;
                    currentMinute -= 60;
                }

                // Add a 5-minute break between classes
                currentMinute += 5;

                // Move to the next day if the schedule exceeds the current day
                if (currentHour >= 14) {
                    currentDay = (currentDay % 5) + 1;
                    currentHour = 8;
                    currentMinute = 30;
                }
            }

            // Reset the count of scheduled classes for the next day
            coursesScheduledPerDay.clear();
        }
    }

    // Display the final schedule with room numbers
    cout << "\nClass Schedule with Room Numbers:\n";
    cout << "+-------------+-------------+-----------------------------+----------------------+\n";
    cout << "|     Day     |  Time Slot  |          Courses           |      Room Number     |\n";
    cout << "+-------------+-------------+-----------------------------+----------------------+\n";

    for (int day = 1; day <= 5; ++day) {
        cout << "| ";
        bool k;

        switch (day) {
            case 1: cout << "Monday    "; break;
            case 2: cout << "Tuesday   "; break;
            case 3: cout << "Wednesday "; break;
            case 4: cout << "Thursday  "; break;
            case 5: cout << "Friday    "; break;
            
        }
        k=false;

        cout << "| ";

        for (int color = 1; color <= chromaticNumber; ++color) {
            if (scheduleWithRooms[day].empty()) {
                cout << "| No class there            |                       No room assigned                   |\n";
                break;
            }

            if (color <= coursesByColor.size()) {
                for (const auto &classInfo : scheduleWithRooms[day]) {
                    if (!scheduleWithRooms[day].empty()) {
                        for (const string &scheduledCourse : coursesByColor[color]) {
                            if (scheduledCourse == classInfo.course) {
                                if (k){
                                    cout<<"|           | ";

                                }
                                k=true;

                                cout << "| " << classInfo.time << " | " << scheduledCourse << " ";
                                for (int i = scheduledCourse.length(); i < 22; ++i) {
                                    cout << " ";
                                }
                                cout << "| " << classInfo.roomNumber;
                                for (int i = to_string(classInfo.roomNumber).length(); i < 21; ++i) {
                                    cout << " ";
                                }
                                cout << "|\n";
                            }
                        }
                    }
                }
            }
        }

        cout << "+-------------+-------------+-----------------------------+----------------------+\n";
    }
}



int main() {
    string fileName;
    cout << "Enter the name of the input file: ";
    cin >> fileName;

    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << fileName << endl;
        return 1;
    }

    int numCourses;
    inputFile >> numCourses;

    vector<string> courseNames(numCourses);
    for (int i = 0; i < numCourses; ++i) {
        inputFile >> courseNames[i];
    }

    unordered_map<string, set<string> > graph;

    for (int i = 0; i < numCourses; ++i) {
        for (int j = i + 1; j < numCourses; ++j) {
            int numCommonStudents;
            inputFile >> numCommonStudents;

            set<string> commonStudents;
            for (int k = 0; k < numCommonStudents; ++k) {
                string studentName;
                inputFile >> studentName;
                commonStudents.insert(studentName);
            }

            if (!commonStudents.empty()) {
                graph[courseNames[i]].insert(courseNames[j]);
                graph[courseNames[j]].insert(courseNames[i]);
            }
        }
    }

    map<string, int> coloring;
    int chromaticNumber = 0;

    for (const auto &entry : graph) {
        const string &node = entry.first;
        if (coloring.find(node) == coloring.end()) {
            colorGraph(graph, coloring, node, chromaticNumber);
        }
    }

    cout << "\nColored graph representation:\n";
    for (const auto &entry : coloring) {
        cout << entry.first << " is colored with color " << entry.second << endl;
    }

    int numRooms;
    cout << "Enter the number of available rooms: ";
    cin >> numRooms;

    vector<int> availableRooms(numRooms);
    cout << "Enter the names of available rooms:\n";
    for (int i = 0; i < numRooms; ++i) {
        cin >> availableRooms[i];
    }

    displayScheduleWithRooms(graph, coloring, chromaticNumber, availableRooms);

    return 0;
}
