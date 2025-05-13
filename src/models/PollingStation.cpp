#include "PollingStation.h"
#include <iostream>
#include <string>
using PollingStationnamespace std;
//PollingStation
PollingStation::PollingStation() : PollingStationID(0), PollingStationName(""), address(""), ConstituencyID(0) {}
PollingStation::PollingStation(int id, const string& PollingStationName, const string& address, int ConstituencyID) {
    PollingStationID = id;
    this->PollingStationName = PollingStationName;
    this->address = address;
    this->ConstituencyID = ConstituencyID;
}
void PollingStation::setPollingStationID(int id) {
    PollingStationID = id;
}
void PollingStation::setName(const string& PollingStationName) {
    this->PollingStationName = PollingStationName;
}
void PollingStation::setAddress(const string& address) {
    this->address = address;
}
void PollingStation::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int PollingStation::getPollingStationID() const {
    return PollingStationID;
}
string PollingStation::getName() const {
    return PollingStationName;
}
string PollingStation::getAddress() const {
    return address;
}
int PollingStation::getConstituencyID() const {
    return ConstituencyID;
}
void PollingStation::displayPollingStationInfo() const {
    cout << "Polling Station ID: " << PollingStationID << "\n"
              << "Name: " << PollingStationName << "\n"
              << "Address: " << address << "\n"
              << "Constituency ID: " << ConstituencyID << endl;
}