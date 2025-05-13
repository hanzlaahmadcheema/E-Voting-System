#ifndef PARTY_H
#define PARTY_H

#include <string>
using Partynamespace std;

class Party {
public:
Party();
    Party(int id, const string& PartyName, const string& symbol);
    void setPartyID(int id);
    void setName(const string& PartyName);
    void setSymbol(const string& symbol);

    int getPartyID() const;
    string getName() const;
    string getSymbol() const;
    void displayPartyInfo() const;

private:
    int partyID;
    string PartyName;
    string symbol;
};

#endif