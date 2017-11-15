#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

typedef vector<vector<int>> planeVec;
typedef unordered_map<string, int> pairMap;

const int NUM_PPL = 300; //full data set = 300
const int THRESH = 50; //counts greater than OR EQUAL will be considered interesting
const int NUM_PLANES = 100; //full data set = 100
const char FILE_NAME[20] = "newlists.csv"; //char[] so that no chance of exception for global (static) assignment



planeVec readListsFile();

/*
 * Algorithm that will scan through each plane looking for a pair. Table keeps track of all plane pairs in the entire data set.
 * When pair count reaches the threshold, that pair is inserted into a hash table.
 */
pairMap searchPairs(const planeVec &planes);

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
    pairMap pairs = searchPairs(planes);
    end = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch());
    cout << "Time to execute algorithm: " << (end.count() - start.count()) << " milliseconds" << endl;

    /*
    //export pairs to a csv for plotting
    const string outFileName = "newlistsThresh50_Out.csv";
    ofstream outFile("../" + outFileName);
    if (outFile.fail()) {
        cerr << "Could not open ../" << outFileName << endl;
        exit(1);
    }
    for (const auto &pair : pairs) {
        string p1 = pair.first.substr(0, pair.first.find(','));
        string p2 = pair.first.substr(pair.first.find(',') + 1);
        outFile << p1 << ',' << p2 << endl;
    }
    outFile.close();
     */
    return 0;
}

planeVec readListsFile() {
    std::string filePath = "../";
    ifstream inFile(filePath + FILE_NAME);
    if (inFile.fail()) {
        cerr << "Could not open " << filePath << FILE_NAME << endl;
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

pairMap searchPairs(const planeVec &planes) {
    static int togetherCount[NUM_PPL][NUM_PPL]; //or not static, use  = {0}; to set to 0, but that is slower
    pairMap pairsAboveThresh;
    for (const auto &plane : planes) {
        //i < plane.size()-1 so that j = i+1 won't get to plane.size()
        int planeSize = plane.size(); //avoid calling plane.size() in each of two following loops
        for (int i = 0; i < planeSize - 1; ++i) {
            int p1 = plane[i]; //first person number in plane
            for (int j = i + 1; j < planeSize; ++j) {
                int p2 = plane[j]; //second person number to pair with the first
                ++togetherCount[p1 - 1][p2 - 1]; //- 1 because the array is 0 to 299 but people numbers are 1 to 300
                //if at threshold, insert into pairs hash table
                //only insert when at threshold - won't have total pair count, but saves computation time
                //if want total count, set == to >= and set paris = THRESH to togetherCount[p1-1][p2-1]
                //This improves speed if you don't care about the number of co-occurrences, only if it is >= THRESH
                //don't check if already in map - if are in map requires 2 accesses instead of 1
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
    cout << "Number of interesting pairs: " << pairsAboveThresh.size() << endl;
    */
    return pairsAboveThresh;
}