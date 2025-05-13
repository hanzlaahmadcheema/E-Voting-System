#ifndef PARTY_H
#define PARTY_H

#include <string>
using namespace std;

class Party {
public:
Party();
    Party(int PartyID, const string& PartyName, const string& PartySymbol);
    void setPartyID(int PartyID);
    void setPartyName(const string& PartyName);
    void setSymbol(const string& PartySymbol);

    int getPartyID() const;
    string getName() const;
    string getSymbol() const;
    void displayPartyInfo() const;

private:
    int PartyID;
    string PartyName;
    string PartySymbol;
};

#endif