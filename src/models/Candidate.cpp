#include "Candidate.h"
#include <iostream>
#include <string>
using namespace std;
//Candidate
Candidate::Candidate() : candidateID(0), name(""), partyID(0), electionID(0), constituencyID(0) {}

Candidate::Candidate(int id, const string& name, int partyID, int electionID, int constituencyID) {
    candidateID = id;
    this->name = name;
    this->partyID = partyID;
    this->electionID = electionID;
    this->constituencyID = constituencyID;
}

void Candidate::setCandidateID(int id) {
    candidateID = id;
}
void Candidate::setName(const string& name) {
    this->name = name;
}
void Candidate::setPartyID(int partyID) {
    this->partyID = partyID;
}
void Candidate::setElectionID(int electionID) {
    this->electionID = electionID;
}
void Candidate::setConstituencyID(int constituencyID) {
    this->constituencyID = constituencyID;
}
int Candidate::getCandidateID() const {
    return candidateID;
}
string Candidate::getName() const {
    return name;
}
int Candidate::getPartyID() const {
    return partyID;
}
int Candidate::getElectionID() const {
    return electionID;
}
int Candidate::getConstituencyID() const {
    return constituencyID;
}

void Candidate::displayCandidateInfo() const {
    cout << "Candidate ID: " << candidateID << "\n"
              << "Name: " << name << "\n"
              << "Party ID: " << partyID << "\n"
              << "Election ID: " << electionID << "\n"
              << "Constituency ID: " << constituencyID << endl;
}
// Add any other methods or member functions as needed