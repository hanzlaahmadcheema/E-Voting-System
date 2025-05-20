#ifndef CITY_H
#define CITY_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class City
{
public:
    City();
    City(int id, const string &CityName, const string &ProvinceName);
    void setCityID(int id);
    void setCityName(const string &CityName);
    void setProvinceName(const string &ProvinceName);
    int getCityID() const;
    string getCityName() const;
    string getProvinceName() const;
    void displayCityInfo() const;

    json toJSON() const;
    static City fromJSON(const json &j);

    static const string CITY_FILE;
    static vector<City> loadAllCities();
    static void saveAllCities(const vector<City> &cities);
    static void addCity(const City &newCity);
    static void editCity(const City &newCity);
    static void deleteCityByID(int cityID);
    static void listAllCities();
    static bool cityExists(int id);
    static void manageCities();

private:
    int CityID;
    string CityName;
    string ProvinceName;
};

#endif