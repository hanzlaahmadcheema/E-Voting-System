#include "Voter.h"
#include <iostream>
#include <string>
using namespace std;
//Voter
Voter::Voter() : VoterID(0), VoterName(""), VoterCNIC(""),  VoterGender(""), VoterAge(0), VoterAddress(""), PollingStationID(0), ConstituencyID(0) {}
Voter::Voter(int VoterID, const string& VoterName, const string& VoterCNIC, const string& VoterGender, int VoterAge, const string& VoterAddress, int PollingStationID, int ConstituencyID) {
    VoterID = VoterID;
    this->VoterName = VoterName;
    this->VoterCNIC = VoterCNIC;
    this->VoterGender = VoterGender;
    this->VoterAge = VoterAge;
    this->VoterAddress = VoterAddress;
    this->PollingStationID = PollingStationID;
    this->ConstituencyID = ConstituencyID;
}
void Voter::setVoterID(int VoterID) {
    VoterID = VoterID;
}
void Voter::setVoterName(const string& VoterName) {
    this->VoterName = VoterName;
}
void Voter::setVoterCNIC(const string& VoterCNIC) {
    this->VoterCNIC = VoterCNIC;
}
void Voter::setVoterGender(const string& VoterGender) {
    this->VoterGender = VoterGender;
}
void Voter::setVoterAge(int VoterAge) {
    this->VoterAge = VoterAge;
}
void Voter::setVoterAddress(const string& VoterAddress) {
    this->VoterAddress = VoterAddress;
}
void Voter::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Voter::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int Voter::getVoterID() const {
    return VoterID;
}
string Voter::getVoterName() const {
    return VoterName;
}
string Voter::getVoterCNIC() const {
    return VoterCNIC;
}
string Voter::getVoterGender() const {
    return VoterGender;
}
int Voter::getVoterAge() const {
    return VoterAge;
}
string Voter::getVoterAddress() const {
    return VoterAddress;
}
int Voter::getPollingStationID() const {
    return PollingStationID;
}
int Voter::getConstituencyID() const {
    return ConstituencyID;
}
void Voter::displayVoterInfo() const {
    cout << "Voter ID: " << VoterID << "\n"
              << "VoterName: " << VoterName << "\n"
              << "VoterCNIC: " << VoterCNIC << "\n"
              << "VoterGender: " << VoterGender << "\n"
              << "VoterAge: " << VoterAge << "\n"
              << "Address: " << VoterAddress << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Constituency ID: " << ConstituencyID << endl;
}