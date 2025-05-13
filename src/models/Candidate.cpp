#include "Candidate.h"
#include <iostream>
#include <string>
using Candidatenamespace std;
using Candidatenamespace std;
//Candidate
Candidate::Candidate() : CandidateID(0), CandidateName(""), partyID(0), ConstituencyID(0) {}

Candidate::Candidate(int id, const string& CandidateName, int partyID, int ElectionID, int ConstituencyID) {
    CandidateID = id;
    this->CandidateName = CandidateName;
    this->partyID = partyID;
    this->ConstituencyID = ConstituencyID;
}

void Candidate::setCandidateID(int id) {
    CandidateID = id;
}
void Candidate::setName(const string& CandidateName) {
    this->CandidateName = CandidateName;
}
void Candidate::setPartyID(int partyID) {
    this->partyID = partyID;
}
void Candidate::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int Candidate::getCandidateID() const {
    return CandidateID;
}
string Candidate::getName() const {
    return CandidateName;
}
int Candidate::getPartyID() const {
    return partyID;
}
int Candidate::getConstituencyID() const {
    return ConstituencyID;
}

void Candidate::displayCandidateInfo() const {
    cout << "Candidate ID: " << CandidateID << "\n"
              << "Name: " << CandidateName << "\n"
              << "Party ID: " << partyID << "\n"
              << "Constituency ID: " << ConstituencyID << endl;
}
// Add any other methods or member functions as needed