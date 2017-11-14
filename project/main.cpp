#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

typedef vector<vector<int>> planeVec;

const int NUM_PPL = 300; //full data set = 300
const int THRESH = 25;
const int NUM_PLANES = 100; //full data set = 100
const char *FILE_NAME = "newlists.csv";

planeVec readListsFile();

void searchPairs(const planeVec &planes);

int main() {
    //setup, needed in all algorithms, don't include in time analysis
    chrono::milliseconds start = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    planeVec planes = readListsFile();
    chrono::milliseconds end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    cout << "Time to take input from file: " << (end.count() - start.count()) << " milliseconds" << endl;

    start = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    searchPairs(planes);
    end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    cout << "Time to execute algorithm: " << (end.count() - start.count()) << " milliseconds" << endl;

    //test with to_string twice with 9 digits and +',' was 0 ms - negligible time for most problem size
    //same for iterating through and setting 300x300 matrix to 0 - plus compiler optimizes {0}
    //[] - https://stackoverflow.com/questions/6897737/using-the-operator-efficiently-with-c-unordered-map
    return 0;
}

planeVec readListsFile() {
    std::string filePath = "../";
    ifstream inFile(filePath + FILE_NAME);
    if (inFile.fail()) {
        cerr << "could not open lists.csv" << endl;
        exit(1); //abort program
    }

    planeVec planes;
    string line;
    int count = 0; //used for testing different data sets, less than all planes
    while (getline(inFile, line) && count < NUM_PLANES) {
        line += '\n';
        vector<int> onePlane;
        string oneNum;
        for (const auto &c : line) {
            if (c != ',' && c != '\n') {
                oneNum += c;
            } else {
                onePlane.push_back(stoi(oneNum));
                oneNum = "";
            }
        }
        planes.push_back(onePlane);
        ++count;
    }

    inFile.close();
    return planes;
}

void searchPairs(const planeVec &planes) {
    int togetherCount[NUM_PPL][NUM_PPL] = {0};
    unordered_map<string, int> pairsAboveThresh;
    for (const auto &plane : planes) {
        for (int i = 0; i < plane.size(); ++i) {
            int p1 = plane[i]; //first person number in plane
            for (int j = i + 1; j < plane.size(); ++j) {
                int p2 = plane[j]; //second person number to pair with the first
                ++togetherCount[p1 - 1][p2 - 1]; //- 1 because the array is 0 to 299, people numbers are 1 to 300
                //if at threshold, insert into pairs hash table
                //only insert when at threshold - won't have total pair count, but saves computation time
                //if want total count, set == to >= and set paris = THRESH to togetherCount[p1-1][p2-1]
                //This improves speed if you don't care about the number of co-occurences, only if it is >= THRESH
                if (togetherCount[p1 - 1][p2 - 1] == THRESH) {
                    string key = to_string(p1) + ',' + to_string(p2);
                    pairsAboveThresh[key] = THRESH;
                }
            }
        }
    }
    /* Removed outputting the pairs for speed test purposes - algorithm doesn't need to print pairs
    for (const auto &pair : pairsAboveThresh) {
        cout << pair.first << " ";
    }
     */
    cout << pairsAboveThresh.size() << endl;
}