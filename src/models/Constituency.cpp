#include "Constituency.h"
#include <iostream>
#include <string>
using Constituencynamespace std;
//Constituency
Constituency::Constituency() : ConstituencyID(0), ConstituencyName(""), cityID(0) {}
Constituency::Constituency(int id, const string& ConstituencyName, int cityID) {
    ConstituencyID = id;
    this->ConstituencyName = ConstituencyName;
    this->cityID = cityID;
}
void Constituency::setConstituencyID(int id) {
    ConstituencyID = id;
}
void Constituency::setName(const string& ConstituencyName) {
    this->ConstituencyName = ConstituencyName;
}
void Constituency::setCityID(int cityID) {
    this->cityID = cityID;
}
int Constituency::getConstituencyID() const {
    return ConstituencyID;
}
string Constituency::getName() const {
    return ConstituencyName;
}
int Constituency::getCityID() const {
    return cityID;
}
void Constituency::displayConstituencyInfo() const {
    cout << "Constituency ID: " << ConstituencyID << "\n"
              << "Name: " << ConstituencyName << "\n"
              << "City ID: " << cityID << endl;
}
// Add any other methods or member functions as needed