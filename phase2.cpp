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

struct ExperimentData {
    double value;
    string timestamp;
};

bool readDataFromFile(const string &filename, vector<ExperimentData> &data) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found. Please try again.\n";
        return false;
    }

    string line;
    double value;
    string timestamp;
    
    while (file >> value >> timestamp) {
        ExperimentData ed;
        ed.value = value;
        ed.timestamp = timestamp;
        data.push_back(ed);
    }
    
    return true;
}

// Sorting functions
bool compareValues(const ExperimentData &a, const ExperimentData &b) {
    return a.value < b.value;
}

// Statistics calculations
double calculateMean(const vector<ExperimentData> &data) {
    double sum = 0;
    for (const auto &d : data) sum += d.value;
    return sum / data.size();
}

double calculateMedian(vector<ExperimentData> data) {
    size_t size = data.size();
    vector<double> values;
    for (const auto &d : data) values.push_back(d.value);
    sort(values.begin(), values.end());
    return size % 2 == 0 ? (values[size/2-1] + values[size/2]) / 2 : values[size/2];
}

double calculateMode(const vector<ExperimentData> &data) {
    map<double, int> frequency;
    for (const auto &d : data) frequency[d.value]++;
    int maxCount = 0;
    double mode = data[0].value;
    for (const auto &entry : frequency) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            mode = entry.first;
        }
    }
    return mode;
}

int main() {
    cout << "*** Welcome to Vandan's Data Analyzer ***\n";
    int fileCount;
    cout << "Enter the number of files to read: ";
    cin >> fileCount;

    vector<ExperimentData> allData;

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
        if (readDataFromFile(filename, fileData)) {
            cout << "The list of " << fileData.size() << " values in file " << filename << " is:\n";
            
            // Print values with timestamps
            for (const auto &d : fileData) {
                cout << fixed << setprecision(2);
                cout << setw(8) << d.value << setw(12) << d.timestamp << "\n";
            }
            
            allData.insert(allData.end(), fileData.begin(), fileData.end());
        } else {
            --i;
        }
    }

    if (allData.empty()) {
        cout << "No valid data found to process. Exiting.\n";
        return 0;
    }

    cout << "\n*** Summarized Statistics ***\n\n";
    
    vector<ExperimentData> sortedData = allData;
    sort(sortedData.begin(), sortedData.end(), compareValues);
    
    cout << "The orderly sorted list of " << sortedData.size() << " values is:\n";
    for (size_t i = 0; i < sortedData.size(); i++) {
        cout << fixed << setprecision(2) << sortedData[i].value;
        if (i < sortedData.size() - 1) cout << " ";
    }
    cout << "\n\n";

    cout << fixed << setprecision(3);
    cout << "The mean is " << calculateMean(sortedData) << "\n";
    cout << "The median is " << calculateMedian(sortedData) << "\n";
    cout << "The mode is " << calculateMode(sortedData) << "\n";

    string outputFilename;
    cout << "\nEnter the output filename to save: ";
    cin >> outputFilename;

    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        outFile << fixed << setprecision(3);
        outFile << "*** Summarized Statistics ***\n\n";
        outFile << "The orderly sorted list of " << sortedData.size() << " values is:\n";
        for (size_t i = 0; i < sortedData.size(); i++) {
            outFile << sortedData[i].value;
            if (i < sortedData.size() - 1) outFile << " ";
        }
        outFile << "\n\n";
        outFile << "The mean is " << calculateMean(sortedData) << "\n";
        outFile << "The median is " << calculateMedian(sortedData) << "\n";
        outFile << "The mode is " << calculateMode(sortedData) << "\n";
        outFile.close();
        cout << "*** File " << outputFilename << " has been written to disk ***\n";
    }

    cout << "*** Goodbye. ***\n";
    return 0;
}