#ifndef CITY_H
#define CITY_H

#include <string>
using namespace std;

class City {
public:
City();
    City(int id, const string& CityName);
    void setCityID(int id);
    void setCityName(const string& CityName);
    int getCityID() const;
    string getName() const;
    void displayCityInfo() const;
private:
    int CityID;
    string CityName;
};

#endif