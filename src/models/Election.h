#ifndef ELECTION_H
#define ELECTION_H

#include <string>

class Election {
public:
Election();
    Election(int id, const std::string& name, const std::string& type, const std::string& date);
    void startElection();
    void endElection();
    // Add getters and setters here

private:
    int electionID;
    std::string name;
    std::string type;
    std::string date;
};

#endif