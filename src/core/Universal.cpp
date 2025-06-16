#include <custom/config.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

const string COUNTER_FILE = "D://E-Voting-System/data/counters.json";

int getNextID(const string &key)
{
    if (key.empty()) {
        throw std::invalid_argument("Key for getNextID cannot be empty.");
    }

    json counters;
    try {
        std::ifstream inFile(COUNTER_FILE);
        if (inFile) {
            try {
                inFile >> counters;
            } catch (const std::exception& e) {
                std::cerr << "Error parsing counters.json: " << e.what() << std::endl;
                counters = json::object();
            }
            inFile.close();
        } else {
            std::cerr << "counters.json not found. Creating a new one." << std::endl;
            counters = json::object();
        }

        if (!counters.is_object()) {
            counters = json::object();
        }

        int current = counters.value(key, 0);
        int nextID = current + 1;
        counters[key] = nextID;

        std::ofstream outFile(COUNTER_FILE);
        if (!outFile) {
            throw std::runtime_error("Failed to open counters.json for writing.");
        }
        outFile << counters.dump(4);
        outFile.close();

        return nextID;
    } catch (const std::exception& e) {
        std::cerr << "Exception in getNextID: " << e.what() << std::endl;
        return -1; // Indicate error
    }
}

string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return result;
}