#include "Vote.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
//Vote
Vote::Vote() : VoteID(0), VoterID(0), CandidateID(0), ElectionID(0), PollingStationID(0), VoteTime("") {}
Vote::Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string& VoteTime) {
    this->VoteID = VoteID;
    this->VoterID = VoterID;
    this->CandidateID = CandidateID;
    this->ElectionID = ElectionID;
    this->PollingStationID = PollingStationID;
    this->VoteTime = VoteTime;
}
void Vote::setVoteID(int VoteID) {
    VoteID = VoteID;
}
void Vote::setVoterID(int VoterID) {
    this->VoterID = VoterID;
}
void Vote::setCandidateID(int CandidateID) {
    this->CandidateID = CandidateID;
}
void Vote::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Vote::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Vote::setTimestamp(const string& VoteTime) {
    this->VoteTime = VoteTime;
}
int Vote::getVoteID() const {
    return VoteID;
}
int Vote::getVoterID() const {
    return VoterID;
}
int Vote::getCandidateID() const {
    return CandidateID;
}
int Vote::getElectionID() const {
    return ElectionID;
}
int Vote::getPollingStationID() const {
    return PollingStationID;
}
string Vote::getTimestamp() const {
    return VoteTime;
}
void Vote::displayVoteInfo() const {
    cout << "Vote ID: " << VoteID << "\n"
              << "Voter ID: " << VoterID << "\n"
              << "Candidate ID: " << CandidateID << "\n"
              << "Election ID: " << ElectionID << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Timestamp: " << VoteTime << endl;
}

json Vote::toJSON() const {
    return json{
        {"VoteID", VoteID},
        {"VoterID", VoterID},
        {"CandidateID", CandidateID},
        {"ElectionID", ElectionID},
        {"PollingStationID", PollingStationID},
        {"VoteTime", VoteTime}
    };
}

Vote Vote::fromJSON(const json& j) {
    return Vote(
        j.at("VoteID").get<int>(),
        j.at("VoterID").get<int>(),
        j.at("CandidateID").get<int>(),
        j.at("ElectionID").get<int>(),
        j.at("PollingStationID").get<int>(),
        j.at("VoteTime").get<std::string>()
    );
}

const string VOTE_FILE = "../../data/votes.json";

// Load all votes
vector<Vote> loadAllVotes() {
    vector<Vote> list;
    ifstream file(VOTE_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            list.push_back(Vote::fromJSON(obj));
        }
    }
    return list;
}

// Save all votes
void saveAllVotes(const vector<Vote>& list) {
    ofstream file(VOTE_FILE);
    json j;
    for (const auto& v : list) {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

// User: Cast vote
bool castVote(const Vote& newVote) {
    vector<Vote> votes = loadAllVotes();
    for (const auto& v : votes) {
        if (v.getVoterID() == newVote.getVoterID() &&
            v.getElectionID() == newVote.getElectionID()) {
            cout << "❌ You have already voted in this election.\n";
            return false;
        }
    }
    votes.push_back(newVote);
    saveAllVotes(votes);
    cout << "✅ Vote cast successfully.\n";
    return true;
}

// Admin: View all votes
void listAllVotes() {
    vector<Vote> votes = loadAllVotes();
    for (const auto& v : votes) {
        cout << "🗳️ VoteID: " << v.getVoteID()
             << " | VoterID: " << v.getVoterID()
             << " | CandidateID: " << v.getCandidateID()
             << " | ElectionID: " << v.getElectionID()
             << " | PollingStationID: " << v.getPollingStationID()
             << " | Time: " << v.getTimestamp() << endl;
    }
}