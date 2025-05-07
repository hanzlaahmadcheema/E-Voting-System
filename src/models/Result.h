#ifndef RESULT_H
#define RESULT_H

#include <string>

class Result {
public:
Result();
    Result(int id, int constituencyID, int electionID, int winnerCandidateID, int totalVotes);
    void declareResult();
    // Add getters and setters here

private:
    int resultID;
    int constituencyID;
    int electionID;
    int winnerCandidateID;
    int totalVotes;
};

#endif