#ifndef VOTE_H
#define VOTE_H

#include <string>

class Vote {
public:
Vote();
    Vote(int id, int voterID, int candidateID, int electionID, int pollingStationID, const std::string& timestamp);
    // Add getters and setters here

private:
    int voteID;
    int voterID;
    int candidateID;
    int electionID;
    int pollingStationID;
    std::string timestamp;
};

#endif