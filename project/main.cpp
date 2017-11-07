#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

typedef vector<vector<int>> planeVec;

const int NUM_PPL = 300;
const int THRESH = 5;

planeVec readListsFile();

void searchPairsAlgo(planeVec planes);

int main() {
    //setup, needed in all algorithms, don't include in time analysis
    planeVec planes = readListsFile();

    chrono::milliseconds start = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    searchPairsAlgo(planes);
    chrono::milliseconds end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());

    cout << "Time to execute: " << (end.count() - start.count()) << " milliseconds" << endl;
    return 0;
}

void searchPairsAlgo(planeVec planes) {
    //data structures to count pairs
    int togetherCount[NUM_PPL][NUM_PPL] = {0};
    unordered_map<string, int> pairsAboveThresh;

    //loop through every person in every plane
    for (const auto &plane : planes) {
        for (int i = 0; i < plane.size() - 1; ++i) {
            for (int j = i + 1; j < plane.size(); ++j) {
                int p1 = plane[i];
                int p2 = plane[j];
                ++togetherCount[p1][p2];
                if (togetherCount[p1][p2] > THRESH) {
                    //make string key - unique person numbers so first concat comma concat second is unique key
                    string key = to_string(p1) + ',' + to_string(p2);
                    //see if pair already above threshold
                    if (pairsAboveThresh.find(key) != pairsAboveThresh.end()) {
                        ++(pairsAboveThresh.find(key)->second); //increase counter
                    } else {
                        //insert new pair with the current count
                        pair<string, int> planePair(key, togetherCount[p1][p2]);
                        pairsAboveThresh.insert(planePair);
                    }
                }
            }
        }
    }
    //go through pairs above the threshold and print them
    for (const auto &pair : pairsAboveThresh) {
        //separate based on comma
        string p1 = pair.first.substr(0, pair.first.find(','));
        string p2 = pair.first.substr(pair.first.find(',') + 1);
        cout << "Person 1: " << p1 << " Person 2: " << p2 << " Count: " << pair.second << endl;
    }
}

planeVec readListsFile() {
    ifstream inFile("../lists.csv");
    if (inFile.fail()) {
        cerr << "could not open lists.csv" << endl;
        exit(1); //abort program
    }

    planeVec planes;
    string line;
    while (getline(inFile, line)) {
        vector<int> onePlane;
        string oneNum;
        for (auto c : line) {
            if (c != ',') {
                oneNum += c;
            } else {
                onePlane.push_back(stoi(oneNum));
                oneNum = "";
            }
        }
        planes.push_back(onePlane);
    }

    inFile.close();
    return planes;
}