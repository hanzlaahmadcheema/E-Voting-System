#include "Election.h"
#include <iostream>
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
void Election::setType(const string& ElectionType) {
    this->ElectionType = ElectionType;
}
void Election::setDate(const string& ElectionDate) {
    this->ElectionDate = ElectionDate;
}
int Election::getElectionID() const {
    return ElectionID;
}
string Election::getName() const {
    return ElectionName;
}
string Election::getType() const {
    return ElectionType;
}
string Election::getDate() const {
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