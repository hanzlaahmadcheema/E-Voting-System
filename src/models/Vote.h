#ifndef VOTE_H
#define VOTE_H

#include <string>

class Vote {
public:
Vote();
    Vote(int id, int voterID, int candidateID, int electionID, int pollingStationID, const std::string& timestamp);
    void setVoteID(int id);
    void setVoterID(int voterID);
    void setCandidateID(int candidateID);
    void setElectionID(int electionID);
    void setPollingStationID(int pollingStationID);
    void setTimestamp(const std::string& timestamp);
    int getVoteID() const;
    int getVoterID() const;
    int getCandidateID() const;
    int getElectionID() const;
    int getPollingStationID() const;
    std::string getTimestamp() const;
    void displayVoteInfo() const;

private:
    int voteID;
    int voterID;
    int candidateID;
    int electionID;
    int pollingStationID;
    std::string timestamp;
};

#endif