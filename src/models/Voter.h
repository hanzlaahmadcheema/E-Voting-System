#ifndef VOTER_H
#define VOTER_H

#include <custom/config.h>


class Voter
{
public:
    Voter();
    Voter(int VoterID, const string &VoterName, const string &VoterCNIC, const string &VoterGender, int VoterAge, const string &VoterAddress, int PollingStationID);
    void setVoterID(int VoterID);
    void setVoterName(const string &VoterName);
    void setVoterCNIC(const string &VoterCNIC);
    void setVoterGender(const string &VoterGender);
    void setVoterAge(int VoterAge);
    void setVoterAddress(const string &VoterAddress);
    void setPollingStationID(int PollingStationID);
    int getVoterID() const;
    string getVoterName() const;
    string getVoterCNIC() const;
    string getVoterGender() const;
    int getVoterAge() const;
    string getVoterAddress() const;
    int getPollingStationID() const;

    json toJSON() const;
    static Voter fromJSON(const json &j);

    static const string VOTER_FILE;
    static vector<Voter> loadAllVoters();
    static void saveAllVoters(const vector<Voter> &voters);
    static void registerVoter(const Voter &newVoter);
    static void editVoterByCNIC(int VoterID, const Voter &updatedVoter);
    static void listAllVoters();
    static void deleteVoterByCNIC(int VoterID);
    static vector<Voter> loginByCNIC(const string &VoterCNIC);
    static void viewProfile(const Voter &v);
    static bool voterExists(int id);
    static void manageVoters();

private:
    int VoterID;
    string VoterName;
    string VoterCNIC;
    string VoterGender;
    int VoterAge;
    string VoterAddress;
    int PollingStationID;
};

#endif