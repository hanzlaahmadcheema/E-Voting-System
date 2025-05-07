#include "Constituency.h"
#include <iostream>
#include <string>
//Constituency
Constituency::Constituency() : constituencyID(0), name(""), cityID(0) {}
Constituency::Constituency(int id, const std::string& name, int cityID) {
    constituencyID = id;
    this->name = name;
    this->cityID = cityID;
}
void Constituency::setConstituencyID(int id) {
    constituencyID = id;
}
void Constituency::setName(const std::string& name) {
    this->name = name;
}
void Constituency::setCityID(int cityID) {
    this->cityID = cityID;
}
int Constituency::getConstituencyID() const {
    return constituencyID;
}
std::string Constituency::getName() const {
    return name;
}
int Constituency::getCityID() const {
    return cityID;
}
void Constituency::displayConstituencyInfo() const {
    std::cout << "Constituency ID: " << constituencyID << "\n"
              << "Name: " << name << "\n"
              << "City ID: " << cityID << std::endl;
}
// Add any other methods or member functions as needed