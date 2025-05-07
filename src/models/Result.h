#ifndef RESULT_H
#define RESULT_H

#include <string>

class Result {
public:
Result();
    Result(int id, int constituencyID, int electionID, int winnerCandidateID, int totalVotes);
    void declareResult();
    void setResultID(int id);
    void setConstituencyID(int constituencyID);
    void setElectionID(int electionID);
    void setWinnerCandidateID(int winnerCandidateID);
    void setTotalVotes(int totalVotes);
    int getResultID() const;
    int getConstituencyID() const;
    int getElectionID() const;
    int getWinnerCandidateID() const;
    int getTotalVotes() const;
    void displayResultInfo() const;

private:
    int resultID;
    int constituencyID;
    int electionID;
    int winnerCandidateID;
    int totalVotes;
};

#endif