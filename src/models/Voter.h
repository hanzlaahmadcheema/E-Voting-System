#ifndef VOTER_H
#define VOTER_H

#include <string>

class Voter {
public:
Voter();
    Voter(int id, const std::string& name, const std::string& cnic, const std::string& gender, int age, const std::string& address, int cityID, int constituencyID);
    void displayVoterInfo() const;
    // Add getters and setters here

private:
    int voterID;
    std::string name;
    std::string cnic;
    std::string gender;
    int age;
    std::string address;
    int cityID;
    int constituencyID;
};

#endif