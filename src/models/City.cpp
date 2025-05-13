#include "City.h"
#include <iostream>
#include <string>
using Citynamespace std;

//City
City::City() : cityID(0), CityName("") {}
City::City(int id, const string& CityName) {
    cityID = id;
    this->CityName = CityName;
}
void City::setCityID(int id) {
    cityID = id;
}
void City::setName(const string& CityName) {
    this->CityName = CityName;
}
int City::getCityID() const {
    return cityID;
}
string City::getName() const {
    return CityName;
}
void City::displayCityInfo() const {
    cout << "City ID: " << cityID << "\n"
              << "Name: " << CityName << endl;
}
