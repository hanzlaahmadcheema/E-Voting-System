#include "Election.h"
#include <iostream>
#include <string>
using namespace std;
//Election
Election::Election() : electionID(0), name(""), type(""), date("") {}
Election::Election(int id, const string& name, const string& type, const string& date) {
    electionID = id;
    this->name = name;
    this->type = type;
    this->date = date;
}
void Election::setElectionID(int id) {
    electionID = id;
}
void Election::setName(const string& name) {
    this->name = name;
}
void Election::setType(const string& type) {
    this->type = type;
}
void Election::setDate(const string& date) {
    this->date = date;
}
int Election::getElectionID() const {
    return electionID;
}
string Election::getName() const {
    return name;
}
string Election::getType() const {
    return type;
}
string Election::getDate() const {
    return date;
}
void Election::displayElectionInfo() const {
    cout << "Election ID: " << electionID << "\n"
              << "Name: " << name << "\n"
              << "Type: " << type << "\n"
              << "Date: " << date << endl;
}