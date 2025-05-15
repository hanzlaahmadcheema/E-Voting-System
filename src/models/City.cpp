#include "City.h"
#include <iostream>
#include <string>
using namespace std;

//City
City::City() : CityID(0), CityName("") {}
City::City(int CityID, const string& CityName) {
    this->CityID = CityID;
    this->CityName = CityName;
}
void City::setCityID(int CityID) {
    this->CityID = CityID;
}
void City::setCityName(const string& CityName) {
    this->CityName = CityName;
}
int City::getCityID() const {
    return CityID;
}
string City::getName() const {
    return CityName;
}
void City::displayCityInfo() const {
    cout << "City ID: " << CityID << "\n"
              << "Name: " << CityName << endl;
}

json City::toJSON() const {
    return json{
        {"CityID", CityID},
        {"CityName", CityName}
    };
}
City City::fromJSON(const json& j) {
    return City(
        j.at("CityID").get<int>(),
        j.at("CityName").get<std::string>()
    );
}