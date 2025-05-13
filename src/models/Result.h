#ifndef RESULT_H
#define RESULT_H

#include <string>
using namespace std;

class Result {
public:
Result();
    Result(int id, int PollingStationID, int ElectionID, int winnerCandidateID, int totalVotes);
    void declareResult();
    void setResultID(int id);
    void setPollingStationID(int PollingStationID);
    void setElectionID(int ElectionID);
    void setWinnerCandidateID(int winnerCandidateID);
    void setTotalVotes(int totalVotes);
    int getResultID() const;
    int getPollingStationID() const;
    int getElectionID() const;
    int getWinnerCandidateID() const;
    int getTotalVotes() const;
    void displayResultInfo() const;

private:
    int resultID;
    int PollingStationID;
    int ElectionID;
    int winnerCandidateID;
    int totalVotes;
};

#endif