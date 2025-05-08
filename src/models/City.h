#ifndef CITY_H
#define CITY_H

#include <string>
using namespace std;

class City {
public:
City();
    City(int id, const string& name);
    void setCityID(int id);
    void setName(const string& name);
    int getCityID() const;
    string getName() const;
    void displayCityInfo() const;
private:
    int cityID;
    string name;
};

#endif