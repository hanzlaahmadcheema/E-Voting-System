#include "Party.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//Party
Party::Party() : PartyID(0), PartyName(""), PartySymbol("") {}
Party::Party(int PartyID, const string& PartyName, const string& PartySymbol) {
    this->PartyID = PartyID;
    this->PartyName = PartyName;
    this->PartySymbol = PartySymbol;
}
void Party::setPartyID(int PartyID) {
    this->PartyID = PartyID;
}
void Party::setPartyName(const string& PartyName) {
    this->PartyName = PartyName;
}
void Party::setPartySymbol(const string& PartySymbol) {
    this->PartySymbol = PartySymbol;
}
int Party::getPartyID() const {
    return PartyID;
}
string Party::getPartyName() const {
    return PartyName;
}
string Party::getPartySymbol() const {
    return PartySymbol;
}
void Party::displayPartyInfo() const {
    cout << "Party ID: " << PartyID << "\n"
              << "Name: " << PartyName << "\n"
              << "Symbol: " << PartySymbol << endl;
}

json Party::toJSON() const {
    return json{
        {"PartyID", PartyID},
        {"PartyName", PartyName},
        {"PartySymbol", PartySymbol}
    };
}

Party Party::fromJSON(const json& j) {
    return Party(
        j.at("PartyID").get<int>(),
        j.at("PartyName").get<std::string>(),
        j.at("PartySymbol").get<std::string>()
    );
}

const string PARTY_FILE = "../../data/parties.json";

// Load all parties
vector<Party> loadAllParties() {
    vector<Party> list;
    ifstream file(PARTY_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            list.push_back(Party::fromJSON(obj));
        }
    }
    return list;
}

// Save all parties
void saveAllParties(const vector<Party>& list) {
    ofstream file(PARTY_FILE);
    json j;
    for (const auto& p : list) {
        j.push_back(p.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add party
void addParty(const Party& p) {
    vector<Party> list = loadAllParties();
    list.push_back(p);
    saveAllParties(list);
    cout << "✅ Party added.\n";
}

// Admin: Edit party
void editParty(int id, const string& name, const string& symbol) {
    vector<Party> list = loadAllParties();
    for (auto& p : list) {
        if (p.getPartyID() == id) {
            p.setPartyName(name);
            p.setPartySymbol(symbol);
            break;
        }
    }
    saveAllParties(list);
    cout << "✏️ Party updated.\n";
}

// Admin: Delete party
void deleteParty(int id) {
    vector<Party> list = loadAllParties();
    auto it = remove_if(list.begin(), list.end(), [id](const Party& p) {
        return p.getPartyID() == id;
    });
    list.erase(it, list.end());
    saveAllParties(list);
    cout << "🗑️ Party deleted.\n";
}

// Admin/User: View all parties
void listAllParties() {
    vector<Party> list = loadAllParties();
    for (const auto& p : list) {
        cout << "🏳️ " << p.getPartyID() << " | " << p.getPartyName() << " | Symbol: " << p.getPartySymbol() << endl;
    }
}