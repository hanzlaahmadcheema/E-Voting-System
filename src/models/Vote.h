#ifndef VOTE_H
#define VOTE_H


#include <custom/config.h>





class Vote
{
public:
    Vote();
    Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string &VoteTime);
    void setVoteID(int VoteID);
    void setVoterID(int VoterID);
    void setCandidateID(int CandidateID);
    void setElectionID(int ElectionID);
    void setPollingStationID(int PollingStationID);
    void setTimestamp(const string &VoteTime);
    int getVoteID() const;
    int getVoterID() const;
    int getCandidateID() const;
    int getElectionID() const;
    int getPollingStationID() const;
    string getTimestamp() const;

    json toJSON() const;
    static Vote fromJSON(const json &j);

    static const string VOTE_FILE;
    static vector<Vote> loadAllVotes();
    static void saveAllVotes(const vector<Vote> &votes);
    static bool castVote(const Vote &newVote);
    static void listAllVotes();
    static bool voteExists(int VoterID, int ElectionID);
    static void manageVoting();
    static string getCurrentTimestamp();

private:
    int VoteID;
    int VoterID;
    int CandidateID;
    int ElectionID;
    int PollingStationID;
    string VoteTime;
};

#endif