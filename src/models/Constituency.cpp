#include "Constituency.h"
#include <iostream>
#include <string>
using namespace std;

//Constituency
Constituency::Constituency() : ConstituencyID(0), ConstituencyName(""), CityID(0) {}
Constituency::Constituency(int ConstituencyID, const string& ConstituencyName, int CityID) {
    this->ConstituencyID = ConstituencyID;
    this->ConstituencyName = ConstituencyName;
    this->CityID = CityID;
}
void Constituency::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
void Constituency::setConstituencyName(const string& ConstituencyName) {
    this->ConstituencyName = ConstituencyName;
}
void Constituency::setCityID(int CityID) {
    this->CityID = CityID;
}
int Constituency::getConstituencyID() const {
    return ConstituencyID;
}
string Constituency::getName() const {
    return ConstituencyName;
}
int Constituency::getCityID() const {
    return CityID;
}
void Constituency::displayConstituencyInfo() const {
    cout << "Constituency ID: " << ConstituencyID << "\n"
              << "Name: " << ConstituencyName << "\n"
              << "City ID: " << CityID << endl;
}
// Add any other methods or member functions as needed
json Constituency::toJSON() const {
    return json{
        {"ConstituencyID", ConstituencyID},
        {"ConstituencyName", ConstituencyName},
        {"CityID", CityID}
    };
}
Constituency Constituency::fromJSON(const json& j) {
    return Constituency(
        j.at("ConstituencyID").get<int>(),
        j.at("ConstituencyName").get<std::string>(),
        j.at("CityID").get<int>()
    );
}