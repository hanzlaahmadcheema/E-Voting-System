#ifndef PARTY_H
#define PARTY_H

#include <string>

class Party {
public:
Party();
    Party(int id, const std::string& name, const std::string& symbol);
    // Add getters and setters here

private:
    int partyID;
    std::string name;
    std::string symbol;
};

#endif