#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <custom/config.h>
class Candidate
{
public:
    Candidate();
    Candidate(int CandidateID, const string &CandidateName, int PartyID, int ConstituencyID, const string &ConstituencyType);
    void displayCandidateInfo() const;
    void setCandidateID(int CandidateID);
    void setCandidateName(const string &CandidateName);
    void setPartyID(int PartyID);
    void setConstituencyID(int ConstituencyID);
    void setConstituencyType(const string &ConstituencyType);

    int getCandidateID() const;
    string getCandidateName() const;
    int getPartyID() const;
    int getConstituencyID() const;
    string getConstituencyType() const;

    json toJSON() const;
    static Candidate fromJSON(const json &j);

    static const string CANDIDATE_FILE;
    static vector<Candidate> loadAllCandidates();
    static void saveAllCandidates(const vector<Candidate> &candidates);
    static void addCandidate(const Candidate &newCandidate);
    static void editCandidate(int CandidateID, const string &newName, int newPartyID, int newConstituencyID);
    static void deleteCandidateByID(int CandidateID);
    static void listAllCandidates();
    static void viewCandidatesByConstituency(int constID);
    static Candidate *getCandidateByID(int CandidateID);
    static bool candidateExists(int id);
    static void manageCandidates();

private:
    int CandidateID;
    string CandidateName;
    int PartyID;
    int ConstituencyID;
    string ConstituencyType;
};

#endif