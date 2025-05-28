#include "City.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern int getNextID(const string &key);

// City
City::City() : CityID(0), CityName(""), ProvinceName("") {}
City::City(int CityID, const string &CityName, const string &ProvinceName)
{
    this->CityID = CityID;
    this->CityName = CityName;
    this->ProvinceName = ProvinceName;
}
void City::setCityID(int CityID)
{
    this->CityID = CityID;
}
void City::setCityName(const string &CityName)
{
    this->CityName = CityName;
}
void City::setProvinceName(const string &ProvinceName)
{
    this->ProvinceName = ProvinceName;
}
int City::getCityID() const
{
    return CityID;
}
string City::getCityName() const
{
    return CityName;
}
string City::getProvinceName() const
{
    return ProvinceName;
}
void City::displayCityInfo() const
{
    cout << "City ID: " << CityID << "\n"
         << "Name: " << CityName << endl
         << "Province: " << ProvinceName << endl;
}

json City::toJSON() const
{
    return json{
        {"CityID", CityID},
        {"CityName", CityName},
        {"ProvinceName", ProvinceName}};
}
City City::fromJSON(const json &j)
{
    return City(
        j.at("CityID").get<int>(),
        j.at("CityName").get<std::string>(),
        j.at("ProvinceName").get<std::string>());
}

const string CITY_FILE = "data/cities.json";

bool cityNameExists(const vector<City> &list, const string &name)
{
    for (const auto &c : list)
    {
        if (c.getCityName() == name)
            return true;
    }
    return false;
}

bool isValidCityName(const string &name)
{
    return !name.empty() && name.length() <= 50 && name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos;
}
bool isValidCityID(int id)
{
    return id > 0;
}

// Load all cities
vector<City> loadAllCities()
{
    vector<City> cities;
    ifstream file(CITY_FILE);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            if (!j.is_array())
            {
                cerr << "Invalid cities data format.\n";
                return cities;
            }
            for (auto &obj : j)
            {
                try
                {
                    cities.push_back(City::fromJSON(obj));
                }
                catch (const std::exception &e)
                {
                    cerr << "Error parsing city: " << e.what() << endl;
                }
            }
        }
        catch (const std::exception &e)
        {
            cerr << "Error reading cities file: " << e.what() << endl;
        }
    }
    else
    {
        cerr << "Could not open cities file for reading.\n";
    }
    return cities;
}

// Save all cities
void saveAllCities(const vector<City> &cities)
{
    ofstream file(CITY_FILE);
    if (!file.is_open())
    {
        cerr << "Could not open cities file for writing.\n";
        return;
    }
    json j = json::array();
    for (const auto &c : cities)
    {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add city
void addCity(const City &newCity)
{
    if (newCity.getCityID() <= 0)
    {
        cerr << "Invalid City ID. Must be positive.\n";
        return;
    }
    if (newCity.getCityName().empty())
    {
        cerr << "City name cannot be empty.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    for (const auto &city : cities)
    {
        if (city.getCityID() == newCity.getCityID())
        {
            cerr << "City ID already exists.\n";
            return;
        }
        if (city.getCityName() == newCity.getCityName())
        {
            cerr << "City name already exists.\n";
            return;
        }
    }
    cities.push_back(newCity);
    saveAllCities(cities);
    cout << "City added successfully.\n";
}

// Admin: Edit city by ID
void editCity(int cityID, const string &newName, const string &newProvinceName)
{
    if (cityID <= 0)
    {
        cerr << "Invalid City ID.\n";
        return;
    }
    if (newName.empty())
    {
        cerr << "New city name cannot be empty.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    bool found = false;
    for (const auto &city : cities)
    {
        if (city.getCityName() == newName)
        {
            cerr << "City name already exists.\n";
            return;
        }
    }
    for (auto &city : cities)
    {
        if (city.getCityID() == cityID)
        {
            city.setCityName(newName);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cerr << "City ID not found.\n";
        return;
    }
    saveAllCities(cities);
    cout << "City updated.\n";
}

// Admin: Delete city by ID
void deleteCityByID(int cityID)
{
    if (cityID <= 0)
    {
        cerr << "Invalid City ID.\n";
        return;
    }
    vector<City> cities = loadAllCities();
    auto it = remove_if(cities.begin(), cities.end(), [cityID](const City &c)
                        { return c.getCityID() == cityID; });
    if (it == cities.end())
    {
        cerr << "City ID not found.\n";
        return;
    }
    cities.erase(it, cities.end());
    saveAllCities(cities);
    cout << "City deleted if existed.\n";
}

// Admin/User: View all cities
void listCitiesByProvince(const string &province)
{
    vector<City> cities = loadAllCities();
    if (cities.empty())
    {
        cout << "No cities found.\n";
        return;
    }
    for (const auto &c : cities)
    {
        if (c.getProvinceName() == province)
        {
            cout << c.getCityID() << " - " << c.getCityName() << endl;
        }
    }
}

void listAllCities()
{
    vector<City> cities = loadAllCities();
    if (cities.empty())
    {
        cout << "No cities found.\n";
        return;
    }
    for (const auto &c : cities)
    {
        cout << c.getCityID() << " - " << c.getCityName() << endl;
    }
}

bool cityExists(int id) {
    vector<City> list = loadAllCities();
    for (const auto& c : list) {
        if (c.getCityID() == id) return true;
    }
    return false;
}

void manageCities() {
    int choice;
    while (true) {
        cout << "\n City Management\n";
        cout << "1. Add City\n";
        cout << "2. View All Cities\n";
        cout << "3. Edit City\n";
        cout << "4. Delete City\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, ProvinceName;
            cin.ignore();
            cout << "Enter City Name: ";
            getline(cin, name);
            if (!isValidCityName(name)) {
                cout << "Invalid City Name.\n";
                continue;
            }
            cout << "Select Province: ";
            cout << "1. Punjab\n2. KPK\n3. Sindh\n4. Balochistan\n";
            int provinceChoice;
            cin >> provinceChoice;
            switch (provinceChoice) {
                case 1: ProvinceName = "Punjab"; break;
                case 2: ProvinceName = "KPK"; break;
                case 3: ProvinceName = "Sindh"; break;
                case 4: ProvinceName = "Balochistan"; break;
                default: cout << "Invalid choice. City not created.\n"; continue;
            }
            City c(getNextID("CityID"), name, ProvinceName);
            addCity(c);
        } else if (choice == 2) {
            listAllCities();
        } else if (choice == 3) {
            int id;
            string name, ProvinceName;
            cout << "List of Cities:\n";
            listAllCities();
            cout << "Enter City ID: ";
            cin >> id;
            if (!isValidCityID(id)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            if (!cityExists(id)) {
                cout << "City ID not found.\n";
                continue;
            }
            cin.ignore();
            cout << "Enter New Name: ";
            getline(cin, name);
            if (!isValidCityName(name)) {
                cout << "Invalid City Name.\n";
                continue;
            }
            cout << "Select Province: ";
            cout << "1. Punjab\n2. KPK\n3. Sindh\n4. Balochistan\n";
            int provinceChoice;
            cin >> provinceChoice;
            switch (provinceChoice) {
                case 1: ProvinceName = "Punjab"; break;
                case 2: ProvinceName = "KPK"; break;
                case 3: ProvinceName = "Sindh"; break;
                case 4: ProvinceName = "Balochistan"; break;
                default: cout << "Invalid choice. City not updated.\n"; continue;
            }
            editCity(id, name, ProvinceName);
        } else if (choice == 4) {
            int id;
            cout << "Enter City ID to delete: ";
            cin >> id;
            if (!isValidCityID(id)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            if (!cityExists(id)) {
                cout << "City ID not found.\n";
                continue;
            }
            deleteCityByID(id);
        } else if (choice == 0) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}

// int main()
// {
//     // Example usage
//     City c1(getNextID("CityID"), "Karachi");
//     addCity(c1);
//     listAllCities();
//     editCity(c1.getCityID(), "Lahore");
//     listAllCities();
//     deleteCityByID(c1.getCityID());
//     listAllCities();
//     return 0;
// }