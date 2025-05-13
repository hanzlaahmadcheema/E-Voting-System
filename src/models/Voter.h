#ifndef VOTER_H
#define VOTER_H

#include <string>
using namespace std;

class Voter {
public:
Voter();
    Voter(int VoterID, const string& VoterName, const string& VoterCNIC, const string& VoterGender, int VoterAge, const string& VoterAddress, int PollingStationID, int ConstituencyID);
    void displayVoterInfo() const;
    void setVoterID(int VoterID);
    void setVoterName(const string& VoterName);
    void setVoterCNIC(const string& VoterCNIC);
    void setVoterGender(const string& VoterGender);
    void setVoterAge(int VoterAge);
    void setVoterAddress(const string& VoterAddress);
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