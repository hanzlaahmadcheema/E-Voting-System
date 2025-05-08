#ifndef VOTE_H
#define VOTE_H

#include <string>
using namespace std;

class Vote {
public:
Vote();
    Vote(int id, int voterID, int candidateID, int electionID, int pollingStationID, const string& timestamp);
    void setVoteID(int id);
    void setVoterID(int voterID);
    void setCandidateID(int candidateID);
    void setElectionID(int electionID);
    void setPollingStationID(int pollingStationID);
    void setTimestamp(const string& timestamp);
    int getVoteID() const;
    int getVoterID() const;
    int getCandidateID() const;
    int getElectionID() const;
    int getPollingStationID() const;
    string getTimestamp() const;
    void displayVoteInfo() const;

private:
    int voteID;
    int voterID;
    int candidateID;
    int electionID;
    int pollingStationID;
    string timestamp;
};

#endif