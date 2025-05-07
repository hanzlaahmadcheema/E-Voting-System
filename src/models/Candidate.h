#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

class Candidate {
public:
Candidate();
    Candidate(int id, const std::string& name, int partyID, int electionID, int constituencyID);
    void displayCandidateInfo() const;
    // Add getters and setters here

private:
    int candidateID;
    std::string name;
    int partyID;
    int electionID;
    int constituencyID;
};

#endif