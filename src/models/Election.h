#ifndef ELECTION_H
#define ELECTION_H

#include <string>
using namespace std;

class Election {
public:
Election();
    Election(int id, const string& name, const string& type, const string& date);
    void startElection();
    void endElection();
    void setElectionID(int id);
    void setName(const string& name);
    void setType(const string& type);
    void setDate(const string& date);
    int getElectionID() const;
    string getName() const;
    string getType() const;
    string getDate() const;
    void displayElectionInfo() const;

private:
    int electionID;
    string name;
    string type;
    string date;
};

#endif