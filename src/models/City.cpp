#include "City.h"
#include <iostream>
#include <fstream>
#include <vector>
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
string City::getCityName() const {
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

const string CITY_FILE = "../../data/cities.json";

// Load all cities
vector<City> loadAllCities() {
    vector<City> cities;
    ifstream file(CITY_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            cities.push_back(City::fromJSON(obj));
        }
    }
    return cities;
}

// Save all cities
void saveAllCities(const vector<City>& cities) {
    ofstream file(CITY_FILE);
    json j;
    for (const auto& c : cities) {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add city
void addCity(const City& newCity) {
    vector<City> cities = loadAllCities();
    cities.push_back(newCity);
    saveAllCities(cities);
    cout << "✅ City added successfully.\n";
}

// Admin: Edit city by ID
void editCity(int cityID, const string& newName) {
    vector<City> cities = loadAllCities();
    for (auto& city : cities) {
        if (city.getCityID() == cityID) {
            city.setCityName(newName);
            break;
        }
    }
    saveAllCities(cities);
    cout << "✏️ City updated.\n";
}

// Admin: Delete city by ID
void deleteCityByID(int cityID) {
    vector<City> cities = loadAllCities();
    auto it = remove_if(cities.begin(), cities.end(), [cityID](const City& c) {
        return c.getCityID() == cityID;
    });
    cities.erase(it, cities.end());
    saveAllCities(cities);
    cout << "🗑️ City deleted if existed.\n";
}

// Admin/User: View all cities
void listAllCities() {
    vector<City> cities = loadAllCities();
    for (const auto& c : cities) {
        cout << "🏙️ " << c.getCityID() << " - " << c.getCityName() << endl;
    }
}