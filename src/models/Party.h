#ifndef PARTY_H
#define PARTY_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class Party
{
public:
    Party();
    Party(int PartyID, const string &PartyName, const string &PartySymbol);
    void setPartyID(int PartyID);
    void setPartyName(const string &PartyName);
    void setPartySymbol(const string &PartySymbol);

    int getPartyID() const;
    string getPartyName() const;
    string getPartySymbol() const;
    void displayPartyInfo() const;

    json toJSON() const;
    static Party fromJSON(const json &j);

    static const string PARTY_FILE;
    static vector<Party> loadAllParties();
    static void saveAllParties(const vector<Party> &parties);
    static void addParty(const Party &newParty);
    static void deletePartyByID(int partyID);
    static void listAllParties();

private:
    int PartyID;
    string PartyName;
    string PartySymbol;
};

#endif