#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>

class Constituency {
public:
Constituency();
    Constituency(int id, const std::string& name, int cityID);
    // Add getters and setters here

private:
    int constituencyID;
    std::string name;
    int cityID;
};

#endif