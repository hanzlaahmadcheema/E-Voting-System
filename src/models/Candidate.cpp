#include "Candidate.h"
#include <iostream>
#include <string>
using namespace std;
//Candidate
Candidate::Candidate() : CandidateID(0), CandidateName(""), PartyID(0), ConstituencyID(0) {}

Candidate::Candidate(int CandidateID, const string& CandidateName, int PartyID, int ConstituencyID) {
    this->CandidateID = CandidateID;
    this->CandidateName = CandidateName;
    this->PartyID = PartyID;
    this->ConstituencyID = ConstituencyID;
}

void Candidate::setCandidateID(int CandidateID) {
    this->CandidateID = CandidateID;
}
void Candidate::setCandidateName(const string& CandidateName) {
    this->CandidateName = CandidateName;
}
void Candidate::setPartyID(int PartyID) {
    this->PartyID = PartyID;
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
    return PartyID;
}
int Candidate::getConstituencyID() const {
    return ConstituencyID;
}

void Candidate::displayCandidateInfo() const {
    cout << "Candidate ID: " << CandidateID << "\n"
              << "Name: " << CandidateName << "\n"
              << "Party ID: " << PartyID << "\n"
              << "Constituency ID: " << ConstituencyID << endl;
}
// Add any other methods or member functions as needed
json Candidate::toJSON() const {
    return json{
        {"CandidateID", CandidateID},
        {"CandidateName", CandidateName},
        {"PartyID", PartyID},
        {"ConstituencyID", ConstituencyID}
    };
}
Candidate Candidate::fromJSON(const json& j) {
    return Candidate(
        j.at("CandidateID").get<int>(),
        j.at("CandidateName").get<std::string>(),
        j.at("PartyID").get<int>(),
        j.at("ConstituencyID").get<int>()
    );
}