#ifndef RESULT_H
#define RESULT_H

#include <custom/config.h>


class Result
{
public:
    Result();
    Result(int ResultID, int PollingStationID, int ElectionID, int WinnerCandidateID, int TotalVotes, int ConstituencyID);
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

    json toJSON() const;
    static Result fromJSON(const json &j);

    static const string RESULT_FILE;
    static vector<Result> loadAllResults();
    static void saveAllResults(const vector<Result> &results);
    static void computeConstituencyResult(int ElectionID, int ConstituencyID);
    static void viewResultByConstituency(int ElectionID, int ConstituencyID);
    // static void CleanOrphanedVotes(const vector<Vote>& votes, const vector<Voter>& voters, const vector<Candidate>& candidates);
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