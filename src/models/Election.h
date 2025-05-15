#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include <include../../json.hpp>

using namespace std;
using json = nlohmann::json;

class Election {
public:
Election();
    Election(int id, const string& ElectionName, const string& ElectionType, const string& ElectionDate);
    void startElection();
    void endElection();
    void setElectionID(int id);
    void setElectionName(const string& ElectionName);
    void setType(const string& ElectionType);
    void setDate(const string& ElectionDate);
    int getElectionID() const;
    string getName() const;
    string getType() const;
    string getDate() const;
    void displayElectionInfo() const;    
    
    json toJSON() const;
    static Election fromJSON(const json& j);

private:
    int ElectionID;
    string ElectionName;
    string ElectionType;
    string ElectionDate;
};

#endif