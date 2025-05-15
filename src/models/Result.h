#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

class Result {
public:
Result();
    Result(int ResultID, int PollingStationID, int ElectionID, int WinnerCandidateID, int TotalVotes);
    void declareResult();
    void setResultID(int ResultID);
    void setPollingStationID(int PollingStationID);
    void setElectionID(int ElectionID);
    void setWinnerCandidateID(int WinnerCandidateID);
    void setTotalVotes(int TotalVotes);
    int getResultID() const;
    int getPollingStationID() const;
    int getElectionID() const;
    int getWinnerCandidateID() const;
    int getTotalVotes() const;
    void displayResultInfo() const;    
    
    json toJSON() const;
    static Result fromJSON(const json& j);

private:
    int ResultID;
    int PollingStationID;
    int ElectionID;
    int WinnerCandidateID;
    int TotalVotes;
};

#endif