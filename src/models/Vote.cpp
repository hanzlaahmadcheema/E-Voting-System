#include "Vote.h"
#include <iostream>
#include <string>
//Vote
Vote::Vote() : voteID(0), voterID(0), candidateID(0), electionID(0), pollingStationID(0), timestamp("") {}
Vote::Vote(int id, int voterID, int candidateID, int electionID, int pollingStationID, const std::string& timestamp) {
    voteID = id;
    this->voterID = voterID;
    this->candidateID = candidateID;
    this->electionID = electionID;
    this->pollingStationID = pollingStationID;
    this->timestamp = timestamp;
}
void Vote::setVoteID(int id) {
    voteID = id;
}
void Vote::setVoterID(int voterID) {
    this->voterID = voterID;
}
void Vote::setCandidateID(int candidateID) {
    this->candidateID = candidateID;
}
void Vote::setElectionID(int electionID) {
    this->electionID = electionID;
}
void Vote::setPollingStationID(int pollingStationID) {
    this->pollingStationID = pollingStationID;
}
void Vote::setTimestamp(const std::string& timestamp) {
    this->timestamp = timestamp;
}
int Vote::getVoteID() const {
    return voteID;
}
int Vote::getVoterID() const {
    return voterID;
}
int Vote::getCandidateID() const {
    return candidateID;
}
int Vote::getElectionID() const {
    return electionID;
}
int Vote::getPollingStationID() const {
    return pollingStationID;
}
std::string Vote::getTimestamp() const {
    return timestamp;
}
void Vote::displayVoteInfo() const {
    std::cout << "Vote ID: " << voteID << "\n"
              << "Voter ID: " << voterID << "\n"
              << "Candidate ID: " << candidateID << "\n"
              << "Election ID: " << electionID << "\n"
              << "Polling Station ID: " << pollingStationID << "\n"
              << "Timestamp: " << timestamp << std::endl;
}
