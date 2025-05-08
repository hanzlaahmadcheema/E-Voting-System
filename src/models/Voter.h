#ifndef VOTER_H
#define VOTER_H

#include <string>
using namespace std;

class Voter {
public:
Voter();
    Voter(int id, const string& name, const string& cnic, const string& gender, int age, const string& address, int cityID, int constituencyID);
    void displayVoterInfo() const;
    void setVoterID(int id);
    void setName(const string& name);
    void setCNIC(const string& cnic);
    void setGender(const string& gender);
    void setAge(int age);
    void setAddress(const string& address);
    void setCityID(int cityID);
    void setConstituencyID(int constituencyID);
    int getVoterID() const;
    string getName() const;
    string getCNIC() const;
    string getGender() const;
    int getAge() const;
    string getAddress() const;
    int getCityID() const;
    int getConstituencyID() const;

private:
    int voterID;
    string name;
    string cnic;
    string gender;
    int age;
    string address;
    int cityID;
    int constituencyID;
};

#endif