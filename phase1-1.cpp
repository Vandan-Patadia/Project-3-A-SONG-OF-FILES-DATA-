#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

const string QUIT_COMMAND = "quit";

// Structure to hold experimental data with optional timestamp
struct ExperimentData {
    double value;
    int hour = -1, minute = -1, second = -1; // Default to -1 if no timestamp
};

// Function to check if data includes timestamps and parse accordingly
bool parseDataLine(const string &line, ExperimentData &data, bool &hasTimestamp) {
    istringstream iss(line);
    string valueStr, timeStr;

    if (hasTimestamp) {
        if (!(iss >> valueStr >> timeStr)) return false;
        replace(timeStr.begin(), timeStr.end(), '.', ' ');
        istringstream timeStream(timeStr);
        if (!(timeStream >> data.hour >> data.minute >> data.second)) return false;
    } else {
        if (!(iss >> valueStr)) return false;
    }

    try {
        data.value = stod(valueStr);
    } catch (...) {
        return false;
    }

    hasTimestamp = hasTimestamp || (data.hour >= 0 && data.minute >= 0 && data.second >= 0);
    return true;
}

// Function to read data from file and check for missing or corrupted data
bool readDataFromFile(const string &filename, vector<ExperimentData> &data, bool &hasTimestamp) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found. Please try again.\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        ExperimentData ed;
        if (parseDataLine(line, ed, hasTimestamp)) {
            data.push_back(ed);
        } else {
            cout << "Not an input file. Illegal content/structure detected in " << filename << ". Please try again.\n";
            return false;
        }
    }
    return true;
}

int main() {
    cout << "*** Welcome to Vandan's Data Analyzer ***\n";
    int fileCount;
    cout << "Enter the number of files to read: ";
    cin >> fileCount;

    vector<ExperimentData> allData;
    bool hasTimestamp = false;

    for (int i = 1; i <= fileCount; ++i) {
        string filename;
        cout << "\nEnter the filename for file " << i << ": ";
        cin >> filename;

        if (filename == QUIT_COMMAND) {
            if (allData.empty()) {
                cout << "No data to process. Exiting.\n";
                return 0;
            }
            break;
        }

        vector<ExperimentData> fileData;
        if (readDataFromFile(filename, fileData, hasTimestamp)) {
            cout << "The list of " << fileData.size() << " values in file " << filename << " is:\n";
            for (const auto& d : fileData) {
                cout << d.value << "\n";
            }
            allData.insert(allData.end(), fileData.begin(), fileData.end());
        } else {
            --i; // Retry this file index if an error occurs
        }
    }

    if (allData.empty()) {
        cout << "No valid data found to process. Exiting.\n";
        return 0;
    }

}