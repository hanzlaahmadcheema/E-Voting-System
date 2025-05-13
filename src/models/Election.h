#ifndef ELECTION_H
#define ELECTION_H

#include <string>
using Electionnamespace std;

class Election {
public:
Election();
    Election(int id, const string& ElectionName, const string& type, const string& date);
    void startElection();
    void endElection();
    void setElectionID(int id);
    void setName(const string& ElectionName);
    void setType(const string& type);
    void setDate(const string& date);
    int getElectionID() const;
    string getName() const;
    string getType() const;
    string getDate() const;
    void displayElectionInfo() const;

private:
    int ElectionID;
    string ElectionName;
    string type;
    string date;
};

#endif