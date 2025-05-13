#include "Result.h"
#include <iostream>
#include <string>
using namespace std;
//Result
Result::Result() : resultID(0), PollingStationID(0), ElectionID(0), winnerCandidateID(0), totalVotes(0) {}
Result::Result(int id, int ConstituencyID, int ElectionID, int winnerCandidateID, int totalVotes) {
    resultID = id;
    this->PollingStationID = ConstituencyID;
    this->ElectionID = ElectionID;
    this->winnerCandidateID = winnerCandidateID;
    this->totalVotes = totalVotes;
}
void Result::declareResult() {
    cout << "Result declared for Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << winnerCandidateID << "\n"
              << "Total Votes: " << totalVotes << endl;
}
void Result::setResultID(int id) {
    resultID = id;
}
void Result::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Result::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Result::setWinnerCandidateID(int winnerCandidateID) {
    this->winnerCandidateID = winnerCandidateID;
}
void Result::setTotalVotes(int totalVotes) {
    this->totalVotes = totalVotes;
}
int Result::getResultID() const {
    return resultID;
}
int Result::getPollingStationID() const {
    return PollingStationID;
}
int Result::getElectionID() const {
    return ElectionID;
}
int Result::getWinnerCandidateID() const {
    return winnerCandidateID;
}
int Result::getTotalVotes() const {
    return totalVotes;
}
void Result::displayResultInfo() const {
    cout << "Result ID: " << resultID << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << winnerCandidateID << "\n"
              << "Total Votes: " << totalVotes << endl;
}
