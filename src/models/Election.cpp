#include "Election.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//Election
Election::Election() : ElectionID(0), ElectionName(""), ElectionType(""), ElectionDate("") {}
Election::Election(int ElectionID, const string& ElectionName, const string& ElectionType, const string& ElectionDate) {
    this->ElectionID = ElectionID;
    this->ElectionName = ElectionName;
    this->ElectionType = ElectionType;
    this->ElectionDate = ElectionDate;
}
void Election::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Election::setElectionName(const string& ElectionName) {
    this->ElectionName = ElectionName;
}
void Election::setElectionType(const string& ElectionType) {
    this->ElectionType = ElectionType;
}
void Election::setElectionDate(const string& ElectionDate) {
    this->ElectionDate = ElectionDate;
}
int Election::getElectionID() const {
    return ElectionID;
}
string Election::getElectionName() const {
    return ElectionName;
}
string Election::getElectionType() const {
    return ElectionType;
}
string Election::getElectionDate() const {
    return ElectionDate;
}
void Election::displayElectionInfo() const {
    cout << "Election ID: " << ElectionID << "\n"
              << "Name: " << ElectionName << "\n"
              << "Type: " << ElectionType << "\n"
              << "Date: " << ElectionDate << endl;
}

json Election::toJSON() const {
    return json{
        {"ElectionID", ElectionID},
        {"ElectionName", ElectionName},
        {"ElectionType", ElectionType},
        {"ElectionDate", ElectionDate}
    };
}
Election Election::fromJSON(const json& j) {
    return Election(
        j.at("ElectionID").get<int>(),
        j.at("ElectionName").get<std::string>(),
        j.at("ElectionType").get<std::string>(),
        j.at("ElectionDate").get<std::string>()
    );
}

const string ELECTION_FILE = "../../data/elections.json";

// Load elections
#include <regex>

// Helper: Validate election fields
bool isValidElectionID(int id) {
    return id > 0;
}
bool isValidElectionName(const string& name) {
    return !name.empty() && name.length() <= 100;
}
bool isValidElectionType(const string& type) {
    return !type.empty() && type.length() <= 50;
}
bool isValidElectionDate(const string& date) {
    // YYYY-MM-DD format
    regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
    return regex_match(date, date_regex);
}

vector<Election> loadAllElections() {
    vector<Election> list;
    ifstream file(ELECTION_FILE);
    if (file.is_open()) {
        json j;
        try {
            file >> j;
            for (auto& obj : j) {
                try {
                    Election e = Election::fromJSON(obj);
                    if (isValidElectionID(e.getElectionID()) &&
                        isValidElectionName(e.getElectionName()) &&
                        isValidElectionType(e.getElectionType()) &&
                        isValidElectionDate(e.getElectionDate())) {
                        list.push_back(e);
                    }
                } catch (...) {
                    // Skip invalid entry
                }
            }
        } catch (...) {
            // Invalid JSON, return empty list
        }
    }
    return list;
}

// Save elections
void saveAllElections(const vector<Election>& list) {
    ofstream file(ELECTION_FILE);
    json j;
    for (const auto& e : list) {
        j.push_back(e.toJSON());
    }
    file << j.dump(4);
}

// Admin: Create election
void createElection(const Election& e) {
    if (!isValidElectionID(e.getElectionID())) {
        cout << "❌ Invalid Election ID.\n";
        return;
    }
    if (!isValidElectionName(e.getElectionName())) {
        cout << "❌ Invalid Election Name.\n";
        return;
    }
    if (!isValidElectionType(e.getElectionType())) {
        cout << "❌ Invalid Election Type.\n";
        return;
    }
    if (!isValidElectionDate(e.getElectionDate())) {
        cout << "❌ Invalid Election Date. Use YYYY-MM-DD.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    for (const auto& existing : list) {
        if (existing.getElectionID() == e.getElectionID()) {
            cout << "❌ Election ID already exists.\n";
            return;
        }
    }
    list.push_back(e);
    saveAllElections(list);
    cout << "🗳️ Election created.\n";
}

// Admin: Edit election
void editElection(int electionID, const string& newName, const string& newType, const string& newDate) {
    if (!isValidElectionID(electionID)) {
        cout << "❌ Invalid Election ID.\n";
        return;
    }
    if (!isValidElectionName(newName)) {
        cout << "❌ Invalid Election Name.\n";
        return;
    }
    if (!isValidElectionType(newType)) {
        cout << "❌ Invalid Election Type.\n";
        return;
    }
    if (!isValidElectionDate(newDate)) {
        cout << "❌ Invalid Election Date. Use YYYY-MM-DD.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    bool found = false;
    for (auto& e : list) {
        if (e.getElectionID() == electionID) {
            e.setElectionName(newName);
            e.setElectionType(newType);
            e.setElectionDate(newDate);
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "❌ Election ID not found.\n";
        return;
    }
    saveAllElections(list);
    cout << "✏️ Election updated.\n";
}

// Admin: Delete election
void deleteElection(int electionID) {
    if (!isValidElectionID(electionID)) {
        cout << "❌ Invalid Election ID.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    auto it = remove_if(list.begin(), list.end(), [electionID](const Election& e) {
        return e.getElectionID() == electionID;
    });
    if (it == list.end()) {
        cout << "❌ Election ID not found.\n";
        return;
    }
    list.erase(it, list.end());
    saveAllElections(list);
    cout << "🗑️ Election deleted.\n";
}

// Admin/User: List all elections
void listAllElections() {
    vector<Election> list = loadAllElections();
    if (list.empty()) {
        cout << "No elections found.\n";
        return;
    }
    for (const auto& e : list) {
        cout << "📆 " << e.getElectionID() << " | " << e.getElectionName()
             << " | " << e.getElectionType() << " | " << e.getElectionDate() << endl;
    }
}

// int main() {
//     // Example usage
//     Election e1(1, "Presidential Election", "Presidential", "2024-11-05");
//     createElection(e1);
//     listAllElections();
//     editElection(1, "Updated Presidential Election", "Presidential", "2024-11-06");  
//     listAllElections();
//     deleteElection(1);
//     listAllElections();
//     return 0;
// }