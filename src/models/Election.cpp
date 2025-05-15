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
vector<Election> loadAllElections() {
    vector<Election> list;
    ifstream file(ELECTION_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            list.push_back(Election::fromJSON(obj));
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
    vector<Election> list = loadAllElections();
    list.push_back(e);
    saveAllElections(list);
    cout << "🗳️ Election created.\n";
}

// Admin: Edit election
void editElection(int electionID, const string& newName, const string& newType, const string& newDate) {
    vector<Election> list = loadAllElections();
    for (auto& e : list) {
        if (e.getElectionID() == electionID) {
            e.setElectionName(newName);
            e.setElectionType(newType);
            e.setElectionDate(newDate);
            break;
        }
    }
    saveAllElections(list);
    cout << "✏️ Election updated.\n";
}

// Admin: Delete election
void deleteElection(int electionID) {
    vector<Election> list = loadAllElections();
    auto it = remove_if(list.begin(), list.end(), [electionID](const Election& e) {
        return e.getElectionID() == electionID;
    });
    list.erase(it, list.end());
    saveAllElections(list);
    cout << "🗑️ Election deleted.\n";
}

// Admin/User: List all elections
void listAllElections() {
    vector<Election> list = loadAllElections();
    for (const auto& e : list) {
        cout << "📆 " << e.getElectionID() << " | " << e.getElectionName()
             << " | " << e.getElectionType() << " | " << e.getElectionDate() << endl;
    }
}