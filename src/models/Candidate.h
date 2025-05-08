#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
using namespace std;
using namespace std;

class Candidate {
public:
Candidate();
    Candidate(int id, const string& name, int partyID, int electionID, int constituencyID);
    void displayCandidateInfo() const;
    void setCandidateID(int id);
    void setName(const string& name);
    void setPartyID(int partyID);
    void setElectionID(int electionID);
    void setConstituencyID(int constituencyID);
    
    int getCandidateID() const;
    string getName() const;
    int getPartyID() const;
    int getElectionID() const;
    int getConstituencyID() const;


private:
    int candidateID;
    string name;
    int partyID;
    int electionID;
    int constituencyID;
};

#endif