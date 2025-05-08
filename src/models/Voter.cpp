#include "Voter.h"
#include <iostream>
#include <string>
using namespace std;
//Voter
Voter::Voter() : voterID(0), name(""), cnic(""),  gender(""), age(0), address(""), cityID(0), constituencyID(0) {}
Voter::Voter(int id, const string& name, const string& cnic, const string& gender, int age, const string& address, int cityID, int constituencyID) {
    voterID = id;
    this->name = name;
    this->cnic = cnic;
    this->gender = gender;
    this->age = age;
    this->address = address;
    this->cityID = cityID;
    this->constituencyID = constituencyID;
}
void Voter::setVoterID(int id) {
    voterID = id;
}
void Voter::setName(const string& name) {
    this->name = name;
}
void Voter::setCNIC(const string& cnic) {
    this->cnic = cnic;
}
void Voter::setGender(const string& gender) {
    this->gender = gender;
}
void Voter::setAge(int age) {
    this->age = age;
}
void Voter::setAddress(const string& address) {
    this->address = address;
}
void Voter::setCityID(int cityID) {
    this->cityID = cityID;
}
void Voter::setConstituencyID(int constituencyID) {
    this->constituencyID = constituencyID;
}
int Voter::getVoterID() const {
    return voterID;
}
string Voter::getName() const {
    return name;
}
string Voter::getCNIC() const {
    return cnic;
}
string Voter::getGender() const {
    return gender;
}
int Voter::getAge() const {
    return age;
}
string Voter::getAddress() const {
    return address;
}
int Voter::getCityID() const {
    return cityID;
}
int Voter::getConstituencyID() const {
    return constituencyID;
}
void Voter::displayVoterInfo() const {
    cout << "Voter ID: " << voterID << "\n"
              << "Name: " << name << "\n"
              << "CNIC: " << cnic << "\n"
              << "Gender: " << gender << "\n"
              << "Age: " << age << "\n"
              << "Address: " << address << "\n"
              << "City ID: " << cityID << "\n"
              << "Constituency ID: " << constituencyID << endl;
}