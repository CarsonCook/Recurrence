#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

typedef vector<vector<int>> planeVec;

const int NUM_PPL = 300; //full data set = 300
const int THRESH = 20;
const int NUM_PLANES = 100; //full data set = 100

planeVec readListsFile();

void searchPairsAlgo(const planeVec &planes);

void markPairsAlgo(const planeVec &planes);

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
    searchPairsAlgo(planes);
    end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    cout << "Time to execute search pairs algo: " << (end.count() - start.count()) << " milliseconds" << endl;

    start = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    markPairsAlgo(planes);
    end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    cout << "Time to execute mark pairs algo: " << (end.count() - start.count()) << " milliseconds" << endl;
    return 0;
}

void markPairsAlgo(const planeVec &planes) {
    unordered_map<string, int> pairs;

    //loop through every person in every plane
    for (const auto &plane : planes) {
        int presentMark[NUM_PPL][NUM_PPL] = {0}; //increment wherever a person is present, if get 2 - both present
        //initialized here so each plane has fresh 0s
        for (auto person : plane) {
            //mark person present for this plane
            for (int i = 0; i < NUM_PPL; ++i) {
                if (i == person - 1) {
                    continue; //don't want case in marking table where person on row and column are the same
                }
                ++presentMark[i][person - 1];
                ++presentMark[person - 1][i];
                //if both present, insert into pairs and start counter
                //or increment counter if already present
                if (presentMark[i][person - 1] >= 2 || presentMark[person - 1][i] >= 2) {
                    string key = to_string(i + 1) + ',' + to_string(person);
                    auto search = pairs.find(key);
                    if (search != pairs.end()) {
                        ++(search->second); //increase counter
                    } else {
                        pair<string, int> planePair(key, 1);
                        pairs.insert(planePair);
                    }
                }
            }
        }
    }
    //go through found pairs and print those above the threshold
    int count = 0;
    for (const auto &pair : pairs) {
        if (pair.second >= THRESH) {
            ++count;
            //separate based on comma
            string p1 = pair.first.substr(0, pair.first.find(','));
            string p2 = pair.first.substr(pair.first.find(',') + 1);
            //when getting the time, ignore actually outputting result as that is slow and
            //is needed for every algorithm. Also doesn't really relate to how efficient algo is.
            // cout << "Person 1: " << p1 << " Person 2: " << p2 << " Count: " << pair.second << endl;
        }
    }
    cout << count << endl; //used to see how many pairs above thesh and ensure both algos get same answer
}

void searchPairsAlgo(const planeVec &planes) {
    //data structures to count pairs
    int togetherCount[NUM_PPL][NUM_PPL] = {0};
    unordered_map<string, int> pairsAboveThresh;

    //loop through every person in every plane
    for (const auto &plane : planes) {
        for (int i = 0; i < plane.size() - 1; ++i) {
            for (int j = i + 1; j < plane.size(); ++j) {
                int p1 = plane[i];
                int p2 = plane[j];
                ++togetherCount[p1 - 1][p2 - 1];
                if (togetherCount[p1 - 1][p2 - 1] >= THRESH) {
                    //make string key - unique person numbers so first concat comma concat second is unique key
                    string key = to_string(p1) + ',' + to_string(p2);
                    //see if pair already above threshold
                    auto search = pairsAboveThresh.find(key);
                    if (search != pairsAboveThresh.end()) {
                        ++(search->second); //increase counter
                    } else {
                        //insert new pair with the current count
                        pair<string, int> planePair(key, togetherCount[p1 - 1][p2 - 1]);
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
        //when getting the time, ignore actually outputting result as that is slow and
        //is needed for every algorithm. Also doesn't really relate to how efficient algo is.
        //  cout << "Person 1: " << p1 << " Person 2: " << p2 << " Count: " << pair.second << endl;
    }
    //used to see number of pairs above thresh, ensure two algos get same answer
    cout << pairsAboveThresh.size() << endl;
}

planeVec readListsFile() {
    ifstream inFile("../lists.csv");
    if (inFile.fail()) {
        cerr << "could not open lists.csv" << endl;
        exit(1); //abort program
    }

    planeVec planes;
    string line;
    int count = 0; //used for testing different data sets, less than all planes
    while (getline(inFile, line) && count < NUM_PLANES) {
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
        ++count;
    }

    inFile.close();
    return planes;
}