#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

class Candidate {
public:
Candidate();
    Candidate(int id, const std::string& name, int partyID, int electionID, int constituencyID);
    void displayCandidateInfo() const;
    // Add getters and setters here
    void setCandidateID(int id);
    void setName(const std::string& name);
    void setPartyID(int partyID);
    void setElectionID(int electionID);
    void setConstituencyID(int constituencyID);
    
    int getCandidateID() const;
    std::string getName() const;
    int getPartyID() const;
    int getElectionID() const;
    int getConstituencyID() const;
    

private:
    int candidateID;
    std::string name;
    int partyID;
    int electionID;
    int constituencyID;
};

#endif