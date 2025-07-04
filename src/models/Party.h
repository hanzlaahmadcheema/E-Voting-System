#ifndef PARTY_H
#define PARTY_H


#include <custom/config.h>





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

    json toJSON() const;
    static Party fromJSON(const json &j);

    static const string PARTY_FILE;
    static vector<Party> loadAllParties();
    static void saveAllParties(const vector<Party> &parties);
    static void addParty(const Party &newParty);
    static void editParty(int partyID, const string &newName, const string &newSymbol);
    static void deleteParty(int partyID);
    static void listAllParties();
    static bool partyExists(int id);
    static void manageParties();


private:
    int PartyID;
    string PartyName;
    string PartySymbol;
};

#endif