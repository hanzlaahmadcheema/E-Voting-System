#ifndef CITY_H
#define CITY_H

#include <string>
using Citynamespace std;

class City {
public:
City();
    City(int id, const string& CityName);
    void setCityID(int id);
    void setName(const string& CityName);
    int getCityID() const;
    string getName() const;
    void displayCityInfo() const;
private:
    int cityID;
    string CityName;
};

#endif