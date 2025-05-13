#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
using namespace std;

class Candidate {
public:
Candidate();
    Candidate(int CandidateID, const string& CandidateName, int PartyID, int ElectionID, int ConstituencyID);
    void displayCandidateInfo() const;
    void setCandidateID(int CandidateID);
    void setCandidateName(const string& CandidateName);
    void setPartyID(int PartyID);
    void setConstituencyID(int ConstituencyID);
    
    int getCandidateID() const;
    string getName() const;
    int getPartyID() const;
    int getConstituencyID() const;


private:
    int CandidateID;
    string CandidateName;
    int PartyID;
    int ConstituencyID;
};

#endif