#ifndef ELECTION_H
#define ELECTION_H


#include <custom/config.h>


class Election
{
public:
    Election();
    Election(int id, const string &ElectionName, const string &ElectionType, const string &ElectionDate);
    void startElection();
    void endElection();
    void setElectionID(int id);
    void setElectionName(const string &ElectionName);
    void setElectionType(const string &ElectionType);
    void setElectionDate(const string &ElectionDate);
    int getElectionID() const;
    string getElectionName() const;
    string getElectionType() const;
    string getElectionDate() const;
    void displayElectionInfo() const;

    json toJSON() const;
    static Election fromJSON(const json &j);

    static const string ELECTION_FILE;
    static vector<Election> loadAllElections();
    static void saveAllElections(const vector<Election> &elections);
    static void createElection(const Election &e);
    static void editElection(int ElectionID, const string &newName, const string &newType, const string &newDate);
    static void deleteElection(int ElectionID);
    static void listAllElections();
    static bool electionExists(int id);
    static void manageElections();

private:
    int ElectionID;
    string ElectionName;
    string ElectionType;
    string ElectionDate;
};

#endif