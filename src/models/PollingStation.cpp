#include "PollingStation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//PollingStation
PollingStation::PollingStation() : PollingStationID(0), PollingStationName(""), PollingStationAddress(""), ConstituencyID(0) {}
PollingStation::PollingStation(int PollingStationID, const string& PollingStationName, const string& PollingStationAddress, int ConstituencyID) {
    this->PollingStationID = PollingStationID;
    this->PollingStationName = PollingStationName;
    this->PollingStationAddress = PollingStationAddress;
    this->ConstituencyID = ConstituencyID;
}
void PollingStation::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void PollingStation::setPollingStationName(const string& PollingStationName) {
    this->PollingStationName = PollingStationName;
}
void PollingStation::setPollingStationAddress(const string& PollingStationAddress) {
    this->PollingStationAddress = PollingStationAddress;
}
void PollingStation::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int PollingStation::getPollingStationID() const {
    return PollingStationID;
}
string PollingStation::getPollingStationName() const {
    return PollingStationName;
}
string PollingStation::getPollingStationAddress() const {
    return PollingStationAddress;
}
int PollingStation::getConstituencyID() const {
    return ConstituencyID;
}
void PollingStation::displayPollingStationInfo() const {
    cout << "Polling Station ID: " << PollingStationID << "\n"
              << "Name: " << PollingStationName << "\n"
              << "Address: " << PollingStationAddress << "\n"
              << "Constituency ID: " << ConstituencyID << endl;
}

json PollingStation::toJSON() const {
    return json{
        {"PollingStationID", PollingStationID},
        {"PollingStationName", PollingStationName},
        {"PollingStationAddress", PollingStationAddress},
        {"ConstituencyID", ConstituencyID}
    };
}

PollingStation PollingStation::fromJSON(const json& j) {
    return PollingStation(
        j.at("PollingStationID").get<int>(),
        j.at("PollingStationName").get<std::string>(),
        j.at("PollingStationAddress").get<std::string>(),
        j.at("ConstituencyID").get<int>()
    );
}

const string STATION_FILE = "../../data/polling_stations.json";

// Load all stations
vector<PollingStation> loadAllStations() {
    vector<PollingStation> list;
    ifstream file(STATION_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            list.push_back(PollingStation::fromJSON(obj));
        }
    }
    return list;
}

// Save all stations
void saveAllStations(const vector<PollingStation>& list) {
    ofstream file(STATION_FILE);
    json j;
    for (const auto& s : list) {
        j.push_back(s.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add station
void addPollingStation(const PollingStation& s) {
    vector<PollingStation> list = loadAllStations();
    list.push_back(s);
    saveAllStations(list);
    cout << "✅ Polling station added.\n";
}

// Admin: Edit station
void editPollingStation(int id, const string& newName, const string& newAddress) {
    vector<PollingStation> list = loadAllStations();
    for (auto& s : list) {
        if (s.getPollingStationID() == id) {
            s.setPollingStationName(newName);
            s.setPollingStationAddress(newAddress);
            break;
        }
    }
    saveAllStations(list);
    cout << "✏️ Polling station updated.\n";
}

// Admin: Delete station
void deletePollingStation(int id) {
    vector<PollingStation> list = loadAllStations();
    auto it = remove_if(list.begin(), list.end(), [id](const PollingStation& s) {
        return s.getPollingStationID() == id;
    });
    list.erase(it, list.end());
    saveAllStations(list);
    cout << "🗑️ Polling station deleted.\n";
}

// Admin/User: View all stations by constituency
void listStationsByConstituency(int constID) {
    vector<PollingStation> list = loadAllStations();
    for (const auto& s : list) {
        if (s.getConstituencyID() == constID) {
            cout << "🏫 " << s.getPollingStationID() << " - " << s.getPollingStationName()
                 << " (" << s.getPollingStationAddress() << ")" << endl;
        }
    }
}