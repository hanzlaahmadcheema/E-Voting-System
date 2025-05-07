#ifndef PARTY_H
#define PARTY_H

#include <string>

class Party {
public:
Party();
    Party(int id, const std::string& name, const std::string& symbol);
    void setPartyID(int id);
    void setName(const std::string& name);
    void setSymbol(const std::string& symbol);

    int getPartyID() const;
    std::string getName() const;
    std::string getSymbol() const;
    void displayPartyInfo() const;

private:
    int partyID;
    std::string name;
    std::string symbol;
};

#endif