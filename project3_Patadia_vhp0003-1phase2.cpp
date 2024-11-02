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

// Sorting functions
bool compareValues(const ExperimentData &a, const ExperimentData &b) {
    return a.value < b.value;
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
 if (allData.empty()) {
        cout << "No valid data found to process. Exiting.\n";
        return 0;
    }

    cout << "*** Goodbye. ***\n";
    return 0;
}
