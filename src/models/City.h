#ifndef CITY_H
#define CITY_H

#include <string>

class City {
public:
City();
    City(int id, const std::string& name);
    void setCityID(int id);
    void setName(const std::string& name);
    int getCityID() const;
    std::string getName() const;
    void displayCityInfo() const;
private:
    int cityID;
    std::string name;
};

#endif