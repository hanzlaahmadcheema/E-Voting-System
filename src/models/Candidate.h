#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

class Candidate {
public:
Candidate();
    Candidate(int CandidateID, const string& CandidateName, int PartyID, int ConstituencyID);
    void displayCandidateInfo() const;
    void setCandidateID(int CandidateID);
    void setCandidateName(const string& CandidateName);
    void setPartyID(int PartyID);
    void setConstituencyID(int ConstituencyID);
    
    int getCandidateID() const;
    string getName() const;
    int getPartyID() const;
    int getConstituencyID() const;

    json toJSON() const;
    static Candidate fromJSON(const json& j);

private:
    int CandidateID;
    string CandidateName;
    int PartyID;
    int ConstituencyID;
};

#endif