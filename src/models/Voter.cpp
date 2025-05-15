#include "Voter.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
//Voter
Voter::Voter() : VoterID(0), VoterName(""), VoterCNIC(""),  VoterGender(""), VoterAge(0), VoterAddress(""), PollingStationID(0), ConstituencyID(0) {}
Voter::Voter(int VoterID, const string& VoterName, const string& VoterCNIC, const string& VoterGender, int VoterAge, const string& VoterAddress, int PollingStationID, int ConstituencyID) {
    this->VoterID = VoterID;
    this->VoterName = VoterName;
    this->VoterCNIC = VoterCNIC;
    this->VoterGender = VoterGender;
    this->VoterAge = VoterAge;
    this->VoterAddress = VoterAddress;
    this->PollingStationID = PollingStationID;
    this->ConstituencyID = ConstituencyID;
}
void Voter::setVoterID(int VoterID) {
    this->VoterID = VoterID;
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

json Voter::toJSON() const {
    return json{
        {"VoterID", VoterID},
        {"VoterName", VoterName},
        {"VoterCNIC", VoterCNIC},
        {"VoterGender", VoterGender},
        {"VoterAge", VoterAge},
        {"VoterAddress", VoterAddress},
        {"PollingStationID", PollingStationID},
        {"ConstituencyID", ConstituencyID}
    };
}

Voter Voter::fromJSON(const json& j) {
    return Voter(
        j.at("VoterID").get<int>(),
        j.at("VoterName").get<std::string>(),
        j.at("VoterCNIC").get<std::string>(),
        j.at("VoterGender").get<std::string>(),
        j.at("VoterAge").get<int>(),
        j.at("VoterAddress").get<std::string>(),
        j.at("PollingStationID").get<int>(),
        j.at("ConstituencyID").get<int>()
    );
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

const string VOTER_FILE = "../../data/voters.json";

// Load all voters from JSON file
vector<Voter> loadAllVoters() {
    vector<Voter> voters;
    ifstream file(VOTER_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            voters.push_back(Voter::fromJSON(obj));
        }
    }
    return voters;
}

// Save all voters to JSON file
void saveAllVoters(const vector<Voter>& voters) {
    ofstream file(VOTER_FILE);
    json j;
    for (const auto& v : voters) {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

// Admin: Register new voter
void registerVoter(const Voter& newVoter) {
    vector<Voter> voters = loadAllVoters();
    voters.push_back(newVoter);
    saveAllVoters(voters);
    cout << "✅ Voter registered successfully.\n";
}

// Admin: List all voters
void listAllVoters() {
    vector<Voter> voters = loadAllVoters();
    for (const auto& v : voters) {
        cout << v.getVoterID() << " | " << v.getVoterName() << " | " << v.getVoterCNIC() << endl;
    }
}

// Admin: Delete voter by ID
void deleteVoterByID(int voterID) {
    vector<Voter> voters = loadAllVoters();
    auto it = remove_if(voters.begin(), voters.end(), [voterID](const Voter& v) {
        return v.getVoterID() == voterID;
    });
    voters.erase(it, voters.end());
    saveAllVoters(voters);
    cout << "🗑️ Voter deleted if existed.\n";
}

// User: Login by CNIC (returns voter object or null)
Voter* loginByCNIC(const string& cnic) {
    vector<Voter> voters = loadAllVoters();
    for (auto& v : voters) {
        if (v.getVoterCNIC() == cnic) {
            return new Voter(v);
        }
    }
    return nullptr;
}

// User: View own profile
void viewProfile(const Voter& v) {
    cout << "👤 Voter Profile\n";
    cout << "ID: " << v.getVoterID() << "\n";
    cout << "Name: " << v.getVoterName() << "\n";
    cout << "CNIC: " << v.getVoterCNIC() << "\n";
    cout << "Gender: " << v.getVoterGender() << "\n";
    cout << "Age: " << v.getVoterAge() << "\n";
    cout << "Address: " << v.getVoterAddress() << "\n";
    cout << "Constituency ID: " << v.getConstituencyID() << "\n";
    cout << "Polling Station ID: " << v.getPollingStationID() << "\n";
}