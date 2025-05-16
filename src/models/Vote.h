#ifndef VOTE_H
#define VOTE_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class Vote
{
public:
    Vote();
    Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, int ConstituencyID, const string &VoteTime);
    void setVoteID(int VoteID);
    void setVoterID(int VoterID);
    void setCandidateID(int CandidateID);
    void setElectionID(int ElectionID);
    void setPollingStationID(int PollingStationID);
    void setConstituencyID(int ConstituencyID);
    void setTimestamp(const string &VoteTime);
    int getVoteID() const;
    int getVoterID() const;
    int getCandidateID() const;
    int getElectionID() const;
    int getPollingStationID() const;
    int getConstituencyID() const;
    string getTimestamp() const;
    void displayVoteInfo() const;

    json toJSON() const;
    static Vote fromJSON(const json &j);

    static const string VOTE_FILE;
    static vector<Vote> loadAllVotes();
    static void saveAllVotes(const vector<Vote> &votes);
    static bool castVote(const Vote &newVote);
    static void listAllVotes();

private:
    int VoteID;
    int VoterID;
    int CandidateID;
    int ElectionID;
    int PollingStationID;
    int ConstituencyID;
    string VoteTime;
};

#endif