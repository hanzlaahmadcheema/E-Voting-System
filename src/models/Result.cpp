#include "Result.h"
#include <iostream>
#include <string>
//Result
Result::Result() : resultID(0), constituencyID(0), electionID(0), winnerCandidateID(0), totalVotes(0) {}
Result::Result(int id, int constituencyID, int electionID, int winnerCandidateID, int totalVotes) {
    resultID = id;
    this->constituencyID = constituencyID;
    this->electionID = electionID;
    this->winnerCandidateID = winnerCandidateID;
    this->totalVotes = totalVotes;
}
void Result::declareResult() {
    std::cout << "Result declared for Election ID: " << electionID << "\n"
              << "Winner Candidate ID: " << winnerCandidateID << "\n"
              << "Total Votes: " << totalVotes << std::endl;
}
void Result::setResultID(int id) {
    resultID = id;
}
void Result::setConstituencyID(int constituencyID) {
    this->constituencyID = constituencyID;
}
void Result::setElectionID(int electionID) {
    this->electionID = electionID;
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
int Result::getConstituencyID() const {
    return constituencyID;
}
int Result::getElectionID() const {
    return electionID;
}
int Result::getWinnerCandidateID() const {
    return winnerCandidateID;
}
int Result::getTotalVotes() const {
    return totalVotes;
}
void Result::displayResultInfo() const {
    std::cout << "Result ID: " << resultID << "\n"
              << "Constituency ID: " << constituencyID << "\n"
              << "Election ID: " << electionID << "\n"
              << "Winner Candidate ID: " << winnerCandidateID << "\n"
              << "Total Votes: " << totalVotes << std::endl;
}
