#ifndef VOTER_H
#define VOTER_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class Voter
{
public:
    Voter();
    Voter(int VoterID, const string &VoterName, const string &VoterCNIC, const string &VoterGender, int VoterAge, const string &VoterAddress, int PollingStationID, int ConstituencyID);
    void setVoterID(int VoterID);
    void setVoterName(const string &VoterName);
    void setVoterCNIC(const string &VoterCNIC);
    void setVoterGender(const string &VoterGender);
    void setVoterAge(int VoterAge);
    void setVoterAddress(const string &VoterAddress);
    void setPollingStationID(int PollingStationID);
    void setConstituencyID(int ConstituencyID);
    int getVoterID() const;
    string getVoterName() const;
    string getVoterCNIC() const;
    string getVoterGender() const;
    int getVoterAge() const;
    string getVoterAddress() const;
    int getPollingStationID() const;
    int getConstituencyID() const;

    json toJSON() const;
    static Voter fromJSON(const json &j);

    static const string VOTER_FILE;
    static vector<Voter> loadAllVoters();
    static void saveAllVoters(const vector<Voter> &voters);
    static void registerVoter(const Voter &newVoter);
    static void editVoterByID(int voterID, const Voter &updatedVoter);
    static void listAllVoters();
    static void deleteVoterByID(int voterID);
    static void updateVoterByID(int voterID, const Voter &updatedVoter);
    static vector<Voter> loginByCNIC(const string &cnic);
    static void viewProfile(const Voter &v);

private:
    int VoterID;
    string VoterName;
    string VoterCNIC;
    string VoterGender;
    int VoterAge;
    string VoterAddress;
    int PollingStationID;
    int ConstituencyID;
};

#endif