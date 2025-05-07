#ifndef VOTER_H
#define VOTER_H

#include <string>

class Voter {
public:
Voter();
    Voter(int id, const std::string& name, const std::string& cnic, const std::string& gender, int age, const std::string& address, int cityID, int constituencyID);
    void displayVoterInfo() const;
    void setVoterID(int id);
    void setName(const std::string& name);
    void setCNIC(const std::string& cnic);
    void setGender(const std::string& gender);
    void setAge(int age);
    void setAddress(const std::string& address);
    void setCityID(int cityID);
    void setConstituencyID(int constituencyID);
    int getVoterID() const;
    std::string getName() const;
    std::string getCNIC() const;
    std::string getGender() const;
    int getAge() const;
    std::string getAddress() const;
    int getCityID() const;
    int getConstituencyID() const;

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