#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
using namespace std;

class Candidate {
public:
Candidate();
    Candidate(int id, const string& CandidateName, int partyID, int ElectionID, int ConstituencyID);
    void displayCandidateInfo() const;
    void setCandidateID(int id);
    void setName(const string& CandidateName);
    void setPartyID(int partyID);
    void setConstituencyID(int ConstituencyID);
    
    int getCandidateID() const;
    string getName() const;
    int getPartyID() const;
    int getConstituencyID() const;


private:
    int CandidateID;
    string CandidateName;
    int partyID;
    int ConstituencyID;
};

#endif