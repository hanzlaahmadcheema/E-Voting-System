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
        try {
            json j;
            file >> j;
            if (!j.is_array()) {
                cerr << "❌ Invalid cities data format.\n";
                return cities;
            }
            for (auto& obj : j) {
                try {
                    cities.push_back(City::fromJSON(obj));
                } catch (const std::exception& e) {
                    cerr << "❌ Error parsing city: " << e.what() << endl;
                }
            }
        } catch (const std::exception& e) {
            cerr << "❌ Error reading cities file: " << e.what() << endl;
        }
    } else {
        cerr << "❌ Could not open cities file for reading.\n";
    }
    return cities;
}

// Save all cities
void saveAllCities(const vector<City>& cities) {
    ofstream file(CITY_FILE);
    if (!file.is_open()) {
        cerr << "❌ Could not open cities file for writing.\n";
        return;
    }
    json j = json::array();
    for (const auto& c : cities) {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add city
void addCity(const City& newCity) {
    if (newCity.getCityID() <= 0) {
        cerr << "❌ Invalid City ID. Must be positive.\n";
        return;
    }
    if (newCity.getCityName().empty()) {
        cerr << "❌ City name cannot be empty.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    for (const auto& city : cities) {
        if (city.getCityID() == newCity.getCityID()) {
            cerr << "❌ City ID already exists.\n";
            return;
        }
        if (city.getCityName() == newCity.getCityName()) {
            cerr << "❌ City name already exists.\n";
            return;
        }
    }
    cities.push_back(newCity);
    saveAllCities(cities);
    cout << "✅ City added successfully.\n";
}

// Admin: Edit city by ID
void editCity(int cityID, const string& newName) {
    if (cityID <= 0) {
        cerr << "❌ Invalid City ID.\n";
        return;
    }
    if (newName.empty()) {
        cerr << "❌ New city name cannot be empty.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    bool found = false;
    for (const auto& city : cities) {
        if (city.getCityName() == newName) {
            cerr << "❌ City name already exists.\n";
            return;
        }
    }
    for (auto& city : cities) {
        if (city.getCityID() == cityID) {
            city.setCityName(newName);
            found = true;
            break;
        }
    }
    if (!found) {
        cerr << "❌ City ID not found.\n";
        return;
    }
    saveAllCities(cities);
    cout << "✏️ City updated.\n";
}

// Admin: Delete city by ID
void deleteCityByID(int cityID) {
    if (cityID <= 0) {
        cerr << "❌ Invalid City ID.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    auto it = remove_if(cities.begin(), cities.end(), [cityID](const City& c) {
        return c.getCityID() == cityID;
    });
    if (it == cities.end()) {
        cerr << "❌ City ID not found.\n";
        return;
    }
    cities.erase(it, cities.end());
    saveAllCities(cities);
    cout << "🗑️ City deleted if existed.\n";
}

// Admin/User: View all cities
void listAllCities() {
    vector<City> cities = loadAllCities();
    if (cities.empty()) {
        cout << "ℹ️ No cities found.\n";
        return;
    }
    for (const auto& c : cities) {
        cout << "🏙️ " << c.getCityID() << " - " << c.getCityName() << endl;
    }
}

// int main() {
//     // Example usage
//     City c1(1, "Karachi");
//     addCity(c1);
//     listAllCities();
//     editCity(1, "Lahore");
//     listAllCities();
//     deleteCityByID(1);
//     listAllCities();
//     return 0;
// }