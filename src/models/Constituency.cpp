#include "Constituency.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//Constituency
Constituency::Constituency() : ConstituencyID(0), ConstituencyName(""), CityID(0) {}
Constituency::Constituency(int ConstituencyID, const string& ConstituencyName, int CityID) {
    this->ConstituencyID = ConstituencyID;
    this->ConstituencyName = ConstituencyName;
    this->CityID = CityID;
}
void Constituency::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
void Constituency::setConstituencyName(const string& ConstituencyName) {
    this->ConstituencyName = ConstituencyName;
}
void Constituency::setCityID(int CityID) {
    this->CityID = CityID;
}
int Constituency::getConstituencyID() const {
    return ConstituencyID;
}
string Constituency::getConstituencyName() const {
    return ConstituencyName;
}
int Constituency::getCityID() const {
    return CityID;
}
void Constituency::displayConstituencyInfo() const {
    cout << "Constituency ID: " << ConstituencyID << "\n"
              << "Name: " << ConstituencyName << "\n"
              << "City ID: " << CityID << endl;
}
// Add any other methods or member functions as needed
json Constituency::toJSON() const {
    return json{
        {"ConstituencyID", ConstituencyID},
        {"ConstituencyName", ConstituencyName},
        {"CityID", CityID}
    };
}
Constituency Constituency::fromJSON(const json& j) {
    return Constituency(
        j.at("ConstituencyID").get<int>(),
        j.at("ConstituencyName").get<std::string>(),
        j.at("CityID").get<int>()
    );
}

const string CONSTITUENCY_FILE = "data/constituencies.json";

// Load all constituencies
vector<Constituency> loadAllConstituencies() {
    vector<Constituency> list;
    ifstream file(CONSTITUENCY_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            list.push_back(Constituency::fromJSON(obj));
        }
    }
    return list;
}

// Save all constituencies
void saveAllConstituencies(const vector<Constituency>& list) {
    ofstream file(CONSTITUENCY_FILE);
    json j;
    for (const auto& c : list) {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add constituency
void addConstituency(const Constituency& newConst) {
    vector<Constituency> list = loadAllConstituencies();
    list.push_back(newConst);
    saveAllConstituencies(list);
    cout << "✅ Constituency added.\n";
}

// Admin: Edit constituency name
void editConstituency(int id, const string& newName) {
    vector<Constituency> list = loadAllConstituencies();
    for (auto& c : list) {
        if (c.getConstituencyID() == id) {
            c.setConstituencyName(newName);
            break;
        }
    }
    saveAllConstituencies(list);
    cout << "✏️ Constituency updated.\n";
}

// Admin: Delete constituency by ID
void deleteConstituency(int id) {
    vector<Constituency> list = loadAllConstituencies();
    auto it = remove_if(list.begin(), list.end(), [id](const Constituency& c) {
        return c.getConstituencyID() == id;
    });
    list.erase(it, list.end());
    saveAllConstituencies(list);
    cout << "🗑️ Constituency deleted if existed.\n";
}

// Admin/User: List by city
void listConstituenciesByCity(int cityID) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getCityID() == cityID) {
            cout << "📍 " << c.getConstituencyID() << " - " << c.getConstituencyName() << endl;
        }
    }
}