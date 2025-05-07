#ifndef ELECTION_H
#define ELECTION_H

#include <string>

class Election {
public:
Election();
    Election(int id, const std::string& name, const std::string& type, const std::string& date);
    void startElection();
    void endElection();
    void setElectionID(int id);
    void setName(const std::string& name);
    void setType(const std::string& type);
    void setDate(const std::string& date);
    int getElectionID() const;
    std::string getName() const;
    std::string getType() const;
    std::string getDate() const;
    void displayElectionInfo() const;

private:
    int electionID;
    std::string name;
    std::string type;
    std::string date;
};

#endif