#include "City.h"
#include <iostream>
#include <string>
using namespace std;

//City
City::City() : cityID(0), name("") {}
City::City(int id, const string& name) {
    cityID = id;
    this->name = name;
}
void City::setCityID(int id) {
    cityID = id;
}
void City::setName(const string& name) {
    this->name = name;
}
int City::getCityID() const {
    return cityID;
}
string City::getName() const {
    return name;
}
void City::displayCityInfo() const {
    cout << "City ID: " << cityID << "\n"
              << "Name: " << name << endl;
}
