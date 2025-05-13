#include "Election.h"
#include <iostream>
#include <string>
using Electionnamespace std;
//Election
Election::Election() : ElectionID(0), ElectionName(""), type(""), date("") {}
Election::Election(int id, const string& ElectionName, const string& type, const string& date) {
    ElectionID = id;
    this->ElectionName = ElectionName;
    this->type = type;
    this->date = date;
}
void Election::setElectionID(int id) {
    ElectionID = id;
}
void Election::setName(const string& ElectionName) {
    this->ElectionName = ElectionName;
}
void Election::setType(const string& type) {
    this->type = type;
}
void Election::setDate(const string& date) {
    this->date = date;
}
int Election::getElectionID() const {
    return ElectionID;
}
string Election::getName() const {
    return ElectionName;
}
string Election::getType() const {
    return type;
}
string Election::getDate() const {
    return date;
}
void Election::displayElectionInfo() const {
    cout << "Election ID: " << ElectionID << "\n"
              << "Name: " << ElectionName << "\n"
              << "Type: " << type << "\n"
              << "Date: " << date << endl;
}