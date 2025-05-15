#ifndef CITY_H
#define CITY_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

class City {
public:
City();
    City(int id, const string& CityName);
    void setCityID(int id);
    void setCityName(const string& CityName);
    int getCityID() const;
    string getName() const;
    void displayCityInfo() const;

    json toJSON() const;
    static City fromJSON(const json& j);

private:
    int CityID;
    string CityName;
};

#endif