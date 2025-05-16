#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include "../../include/json.hpp"
using namespace std;
using json = nlohmann::json;

class Candidate
{
public:
    Candidate();
    Candidate(int CandidateID, const string &CandidateName, int PartyID, int ConstituencyID);
    void displayCandidateInfo() const;
    void setCandidateID(int CandidateID);
    void setCandidateName(const string &CandidateName);
    void setPartyID(int PartyID);
    void setConstituencyID(int ConstituencyID);

    int getCandidateID() const;
    string getCandidateName() const;
    int getPartyID() const;
    int getConstituencyID() const;

    json toJSON() const;
    static Candidate fromJSON(const json &j);

    static const string CANDIDATE_FILE;
    static vector<Candidate> loadAllCandidates();
    static void saveAllCandidates(const vector<Candidate> &candidates);
    static void addCandidate(const Candidate &newCandidate);
    static void editCandidate(int candidateID, const string &newName, int newPartyID, int newConstituencyID);
    static void deleteCandidateByID(int candidateID);
    static void listAllCandidates();
    static void viewCandidatesByConstituency(int constID);
    static Candidate *getCandidateByID(int candidateID);

private:
    int CandidateID;
    string CandidateName;
    int PartyID;
    int ConstituencyID;
};

#endif