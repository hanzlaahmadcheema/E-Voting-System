#include "City.h"
#include <iostream>
#include <string>

//City
City::City() : cityID(0), name("") {}
City::City(int id, const std::string& name) {
    cityID = id;
    this->name = name;
}
void City::setCityID(int id) {
    cityID = id;
}
void City::setName(const std::string& name) {
    this->name = name;
}
int City::getCityID() const {
    return cityID;
}
std::string City::getName() const {
    return name;
}
void City::displayCityInfo() const {
    std::cout << "City ID: " << cityID << "\n"
              << "Name: " << name << std::endl;
}
