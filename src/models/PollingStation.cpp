#include "PollingStation.h"
#include <iostream>
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