#include "Voter.h"
#include <iostream>
#include <string>
//Voter
Voter::Voter() : voterID(0), name(""), cnic(""),  gender(""), age(0), address(""), cityID(0), constituencyID(0) {}
Voter::Voter(int id, const std::string& name, const std::string& cnic, const std::string& gender, int age, const std::string& address, int cityID, int constituencyID) {
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
void Voter::setName(const std::string& name) {
    this->name = name;
}
void Voter::setCNIC(const std::string& cnic) {
    this->cnic = cnic;
}
void Voter::setGender(const std::string& gender) {
    this->gender = gender;
}
void Voter::setAge(int age) {
    this->age = age;
}
void Voter::setAddress(const std::string& address) {
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
std::string Voter::getName() const {
    return name;
}
std::string Voter::getCNIC() const {
    return cnic;
}
std::string Voter::getGender() const {
    return gender;
}
int Voter::getAge() const {
    return age;
}
std::string Voter::getAddress() const {
    return address;
}
int Voter::getCityID() const {
    return cityID;
}
int Voter::getConstituencyID() const {
    return constituencyID;
}
void Voter::displayVoterInfo() const {
    std::cout << "Voter ID: " << voterID << "\n"
              << "Name: " << name << "\n"
              << "CNIC: " << cnic << "\n"
              << "Gender: " << gender << "\n"
              << "Age: " << age << "\n"
              << "Address: " << address << "\n"
              << "City ID: " << cityID << "\n"
              << "Constituency ID: " << constituencyID << std::endl;
}