#include "Universal.h"
#include <fstream>
#include <unordered_map>
#include "../../include/json.hpp"
#include <iostream>

using namespace std;
using json = nlohmann::json;

const string COUNTER_FILE = "data/counters.json";

int getNextID(const string &key)
{
    ifstream inFile(COUNTER_FILE);
    json counters;

    if (inFile)
    {
        inFile >> counters;
        inFile.close();
    }
    else
    {
        cerr << "counters.json not found. Creating a new one.\n";
    }

    // Default to 0 if key not found
    if (!counters.is_object())
    {
        counters = json::object(); // create empty object if file is empty or missing
    }

    int current = counters.value(key, 0);
    int nextID = current + 1;
    counters[key] = nextID;

    ofstream outFile(COUNTER_FILE);
    outFile << counters.dump(4);
    outFile.close();

    return nextID;
}

string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return result;
}