#ifndef VOTE_H
#define VOTE_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

class Vote {
public:
Vote();
    Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string& VoteTime);
    void setVoteID(int VoteID);
    void setVoterID(int VoterID);
    void setCandidateID(int CandidateID);
    void setElectionID(int ElectionID);
    void setPollingStationID(int PollingStationID);
    void setTimestamp(const string& VoteTime);
    int getVoteID() const;
    int getVoterID() const;
    int getCandidateID() const;
    int getElectionID() const;
    int getPollingStationID() const;
    string getTimestamp() const;
    void displayVoteInfo() const;    
    
    json toJSON() const;
    static Vote fromJSON(const json& j);

private:
    int VoteID;
    int VoterID;
    int CandidateID;
    int ElectionID;
    int PollingStationID;
    string VoteTime;
};

#endif