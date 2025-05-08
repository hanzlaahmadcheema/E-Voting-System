#ifndef PARTY_H
#define PARTY_H

#include <string>
using namespace std;

class Party {
public:
Party();
    Party(int id, const string& name, const string& symbol);
    void setPartyID(int id);
    void setName(const string& name);
    void setSymbol(const string& symbol);

    int getPartyID() const;
    string getName() const;
    string getSymbol() const;
    void displayPartyInfo() const;

private:
    int partyID;
    string name;
    string symbol;
};

#endif