#include "Result.h"
#include <iostream>
#include <string>
using namespace std;
//Result
Result::Result() : ResultID(0), PollingStationID(0), ElectionID(0), WinnerCandidateID(0), TotalVotes(0) {}
Result::Result(int ResultID, int ConstituencyID, int ElectionID, int WinnerCandidateID, int TotalVotes) {
    this->ResultID = ResultID;
    this->PollingStationID = ConstituencyID;
    this->ElectionID = ElectionID;
    this->WinnerCandidateID = WinnerCandidateID;
    this->TotalVotes = TotalVotes;
}
void Result::declareResult() {
    cout << "Result declared for Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << WinnerCandidateID << "\n"
              << "Total Votes: " << TotalVotes << endl;
}
void Result::setResultID(int ResultID) {
    this->ResultID = ResultID;
}
void Result::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Result::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Result::setWinnerCandidateID(int WinnerCandidateID) {
    this->WinnerCandidateID = WinnerCandidateID;
}
void Result::setTotalVotes(int TotalVotes) {
    this->TotalVotes = TotalVotes;
}
int Result::getResultID() const {
    return ResultID;
}
int Result::getPollingStationID() const {
    return PollingStationID;
}
int Result::getElectionID() const {
    return ElectionID;
}
int Result::getWinnerCandidateID() const {
    return WinnerCandidateID;
}
int Result::getTotalVotes() const {
    return TotalVotes;
}
void Result::displayResultInfo() const {
    cout << "Result ID: " << ResultID << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << WinnerCandidateID << "\n"
              << "Total Votes: " << TotalVotes << endl;
}
