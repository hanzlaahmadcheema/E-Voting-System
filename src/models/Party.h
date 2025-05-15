#ifndef PARTY_H
#define PARTY_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

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
    
    json toJSON() const;
    static Party fromJSON(const json& j);

private:
    int PartyID;
    string PartyName;
    string PartySymbol;
};

#endif