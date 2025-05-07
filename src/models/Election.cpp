#include "Election.h"
#include <iostream>
#include <string>
//Election
Election::Election() : electionID(0), name(""), type(""), date("") {}
Election::Election(int id, const std::string& name, const std::string& type, const std::string& date) {
    electionID = id;
    this->name = name;
    this->type = type;
    this->date = date;
}
void Election::setElectionID(int id) {
    electionID = id;
}
void Election::setName(const std::string& name) {
    this->name = name;
}
void Election::setType(const std::string& type) {
    this->type = type;
}
void Election::setDate(const std::string& date) {
    this->date = date;
}
int Election::getElectionID() const {
    return electionID;
}
std::string Election::getName() const {
    return name;
}
std::string Election::getType() const {
    return type;
}
std::string Election::getDate() const {
    return date;
}
void Election::displayElectionInfo() const {
    std::cout << "Election ID: " << electionID << "\n"
              << "Name: " << name << "\n"
              << "Type: " << type << "\n"
              << "Date: " << date << std::endl;
}

