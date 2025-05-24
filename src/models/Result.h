#ifndef RESULT_H
#define RESULT_H

#include <string>
#include "../../include/json.hpp"
#include "Voter.h"
#include "Vote.h"
#include "Candidate.h"

using namespace std;
using json = nlohmann::json;

class Result
{
public:
    Result();
    Result(int ResultID, int PollingStationID, int ElectionID, int WinnerCandidateID, int TotalVotes, int ConstituencyID);
    void declareResult();
    void setResultID(int ResultID);
    void setPollingStationID(int PollingStationID);
    void setElectionID(int ElectionID);
    void setWinnerCandidateID(int WinnerCandidateID);
    void setTotalVotes(int TotalVotes);
    void setConstituencyID(int ConstituencyID);
    int getResultID() const;
    int getPollingStationID() const;
    int getElectionID() const;
    int getWinnerCandidateID() const;
    int getTotalVotes() const;
    int getConstituencyID() const;
    void displayResultInfo() const;

    json toJSON() const;
    static Result fromJSON(const json &j);

    static const string RESULT_FILE;
    static vector<Result> loadAllResults();
    static void saveAllResults(const vector<Result> &results);
    static void computeConstituencyResult(int ElectionID, int ConstituencyID);
    static void viewResultByConstituency(int ElectionID, int ConstituencyID);
    static void CleanOrphanedVotes(const std::vector<Vote>& votes, const std::vector<Voter>& voters, const std::vector<Candidate>& candidates);
    static void listAllResults();
    static void manageResults();

private:
    int ResultID;
    int PollingStationID;
    int ElectionID;
    int WinnerCandidateID;
    int TotalVotes;
    int ConstituencyID;
};

#endif