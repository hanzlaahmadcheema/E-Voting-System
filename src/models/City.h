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
    City(int id, const string &CityName);
    void setCityID(int id);
    void setCityName(const string &CityName);
    int getCityID() const;
    string getCityName() const;
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

private:
    int CityID;
    string CityName;
};

#endif