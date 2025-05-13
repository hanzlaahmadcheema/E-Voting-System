#include "Vote.h"
#include <iostream>
#include <string>
using namespace std;
//Vote
Vote::Vote() : VoteID(0), VoterID(0), CandidateID(0), ElectionID(0), PollingStationID(0), VoteTime("") {}
Vote::Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string& VoteTime) {
    this->VoteID = VoteID;
    this->VoterID = VoterID;
    this->CandidateID = CandidateID;
    this->ElectionID = ElectionID;
    this->PollingStationID = PollingStationID;
    this->VoteTime = VoteTime;
}
void Vote::setVoteID(int VoteID) {
    VoteID = VoteID;
}
void Vote::setVoterID(int VoterID) {
    this->VoterID = VoterID;
}
void Vote::setCandidateID(int CandidateID) {
    this->CandidateID = CandidateID;
}
void Vote::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Vote::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Vote::setTimestamp(const string& VoteTime) {
    this->VoteTime = VoteTime;
}
int Vote::getVoteID() const {
    return VoteID;
}
int Vote::getVoterID() const {
    return VoterID;
}
int Vote::getCandidateID() const {
    return CandidateID;
}
int Vote::getElectionID() const {
    return ElectionID;
}
int Vote::getPollingStationID() const {
    return PollingStationID;
}
string Vote::getTimestamp() const {
    return VoteTime;
}
void Vote::displayVoteInfo() const {
    cout << "Vote ID: " << VoteID << "\n"
              << "Voter ID: " << VoterID << "\n"
              << "Candidate ID: " << CandidateID << "\n"
              << "Election ID: " << ElectionID << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Timestamp: " << VoteTime << endl;
}
