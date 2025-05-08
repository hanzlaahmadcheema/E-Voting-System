#include "PollingStation.h"
#include <iostream>
#include <string>
using namespace std;
//PollingStation
PollingStation::PollingStation() : pollingStationID(0), name(""), address(""), constituencyID(0) {}
PollingStation::PollingStation(int id, const string& name, const string& address, int constituencyID) {
    pollingStationID = id;
    this->name = name;
    this->address = address;
    this->constituencyID = constituencyID;
}
void PollingStation::setPollingStationID(int id) {
    pollingStationID = id;
}
void PollingStation::setName(const string& name) {
    this->name = name;
}
void PollingStation::setAddress(const string& address) {
    this->address = address;
}
void PollingStation::setConstituencyID(int constituencyID) {
    this->constituencyID = constituencyID;
}
int PollingStation::getPollingStationID() const {
    return pollingStationID;
}
string PollingStation::getName() const {
    return name;
}
string PollingStation::getAddress() const {
    return address;
}
int PollingStation::getConstituencyID() const {
    return constituencyID;
}
void PollingStation::displayPollingStationInfo() const {
    cout << "Polling Station ID: " << pollingStationID << "\n"
              << "Name: " << name << "\n"
              << "Address: " << address << "\n"
              << "Constituency ID: " << constituencyID << endl;
}