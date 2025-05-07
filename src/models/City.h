#ifndef CITY_H
#define CITY_H

#include <string>

class City {
public:
City();
    City(int id, const std::string& name);
    // Add getters and setters here

private:
    int cityID;
    std::string name;
};

#endif