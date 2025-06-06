#include "Vote.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;
using namespace ftxui;

extern int getNextID(const string &key);
extern int ShowMenu(ScreenInteractive & screen, 
     const std::string& heading, 
     const std::vector<std::string>& options);

// Vote
Vote::Vote() : VoteID(0), VoterID(0), CandidateID(0), ElectionID(0), PollingStationID(0), VoteTime("") {}
Vote::Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string &VoteTime)
{
    this->VoteID = VoteID;
    this->VoterID = VoterID;
    this->CandidateID = CandidateID;
    this->ElectionID = ElectionID;
    this->PollingStationID = PollingStationID;
    this->VoteTime = VoteTime;
}
void Vote::setVoteID(int VoteID)
{
    this->VoteID = VoteID;
}
void Vote::setVoterID(int VoterID)
{
    this->VoterID = VoterID;
}
void Vote::setCandidateID(int CandidateID)
{
    this->CandidateID = CandidateID;
}
void Vote::setElectionID(int ElectionID)
{
    this->ElectionID = ElectionID;
}
void Vote::setPollingStationID(int PollingStationID)
{
    this->PollingStationID = PollingStationID;
}
void Vote::setTimestamp(const string &VoteTime)
{
    this->VoteTime = VoteTime;
}
int Vote::getVoteID() const
{
    return VoteID;
}
int Vote::getVoterID() const
{
    return VoterID;
}
int Vote::getCandidateID() const
{
    return CandidateID;
}
int Vote::getElectionID() const
{
    return ElectionID;
}
int Vote::getPollingStationID() const
{
    return PollingStationID;
}
string Vote::getTimestamp() const
{
    return VoteTime;
}
void Vote::displayVoteInfo() const
{
    cout << "Vote ID: " << VoteID << "\n"
         << "Voter ID: " << VoterID << "\n"
         << "Candidate ID: " << CandidateID << "\n"
         << "Election ID: " << ElectionID << "\n"
         << "Polling Station ID: " << PollingStationID << "\n"
         << "Timestamp: " << VoteTime << endl;
}

json Vote::toJSON() const
{
    return json{
        {"VoteID", VoteID},
        {"VoterID", VoterID},
        {"CandidateID", CandidateID},
        {"ElectionID", ElectionID},
        {"PollingStationID", PollingStationID},
        {"VoteTime", VoteTime}};
}

Vote Vote::fromJSON(const json &j)
{
    return Vote(
        j.at("VoteID").get<int>(),
        j.at("VoterID").get<int>(),
        j.at("CandidateID").get<int>(),
        j.at("ElectionID").get<int>(),
        j.at("PollingStationID").get<int>(),
        j.at("VoteTime").get<std::string>());
}

const string VOTE_FILE = "data/votes.json";

// Load all votes
#include <set>
#include <regex>

// Helper: Validate Vote fields
bool isValidVote(const Vote &vote, const vector<Vote> &existingVotes, string &errorMsg)
{
    // Check for positive IDs
    if (vote.getVoteID() <= 0)
    {
        errorMsg = "VoteID must be positive.";
        return false;
    }
    if (vote.getVoterID() <= 0)
    {
        errorMsg = "VoterID must be positive.";
        return false;
    }
    if (vote.getCandidateID() <= 0)
    {
        errorMsg = "CandidateID must be positive.";
        return false;
    }
    if (vote.getElectionID() <= 0)
    {
        errorMsg = "ElectionID must be positive.";
        return false;
    }
    if (vote.getPollingStationID() <= 0)
    {
        errorMsg = "PollingStationID must be positive.";
        return false;
    }
    // Check for duplicate VoteID
    for (const auto &v : existingVotes)
    {
        if (v.getVoteID() == vote.getVoteID())
        {
            errorMsg = "Duplicate VoteID.";
            return false;
        }
    }
    // Check for duplicate vote by same voter in same election
    for (const auto &v : existingVotes)
    {
        if (v.getVoterID() == vote.getVoterID() && v.getElectionID() == vote.getElectionID())
        {
            errorMsg = "You have already voted in this election.";
            return false;
        }
    }
    // Check timestamp format (basic ISO 8601 check)
    std::regex iso8601(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    if (!std::regex_search(vote.getTimestamp(), iso8601))
    {
        errorMsg = "Invalid timestamp format. Use YYYY-MM-DD HH:MM:SS";
        return false;
    }
    // All checks passed
    return true;
}

string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buffer);
}

vector<Vote> loadAllVotes()
{
    vector<Vote> list;
    ifstream file(VOTE_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                list.push_back(Vote::fromJSON(obj));
            }
        }
        catch (const std::exception &e)
        {
            cerr << "Error loading votes: " << e.what() << endl;
        }
    }
    return list;
}

// Save all votes
void saveAllVotes(const vector<Vote> &list)
{
    ofstream file(VOTE_FILE);
    json j;
    for (const auto &v : list)
    {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

bool voteExists(int VoterID, int ElectionID) {
    vector<Vote> list = loadAllVotes();
    for (const auto& v : list) {
        if (v.getVoterID() == VoterID && v.getElectionID() == ElectionID)
            return true;
    }
    return false;
}

// User: Cast vote
bool castVote(const Vote &newVote)
{
    vector<Vote> votes = loadAllVotes();
    string errorMsg;
    if (!isValidVote(newVote, votes, errorMsg))
    {
        cout << "Vote not cast: " << errorMsg << endl;
        return false;
    }
    votes.push_back(newVote);
    saveAllVotes(votes);
    cout << "Vote cast successfully.\n";
    return true;
}

void deleteVotesByVoterID(int VoterID){
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [VoterID](const Vote &v)
                        { return v.getVoterID() == VoterID; });
    votes.erase(it, votes.end());
    saveAllVotes(votes);
    size_t after = votes.size();
    if (after < before)
        cout << "Votes deleted for Voter ID: " << VoterID << endl;
    else
        cout << "No votes found for Voter ID: " << VoterID << endl;
}

void deleteVotesByCandidateID(int CandidateID){
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [CandidateID](const Vote &v)
                        { return v.getCandidateID() == CandidateID; });
    votes.erase(it, votes.end());
    saveAllVotes(votes);
    size_t after = votes.size();
    if (after < before)
        cout << "Votes deleted for Candidate ID: " << CandidateID << endl;
    else
        cout << "No votes found for Candidate ID: " << CandidateID << endl;
}

void deleteVotesByElectionID(int ElectionID){
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [ElectionID](const Vote &v)
                        { return v.getElectionID() == ElectionID; });
    votes.erase(it, votes.end());
    saveAllVotes(votes);
    size_t after = votes.size();
    if (after < before)
        cout << "Votes deleted for Election ID: " << ElectionID << endl;
    else
        cout << "No votes found for Election ID: " << ElectionID << endl;
}

// Admin: View all votes
void listAllVotes()
{
    vector<Vote> votes = loadAllVotes();
    set<int> seenVoteIDs;
    for (const auto &v : votes)
    {
        // Extra: Validate each vote before displaying
        string errorMsg;
        if (!isValidVote(v, {}, errorMsg))
        {
            cout << "Invalid vote (VoteID: " << v.getVoteID() << "): " << errorMsg << endl;
            continue;
        }
        // Check for duplicate VoteID in file
        if (seenVoteIDs.count(v.getVoteID()))
        {
            cout << "Duplicate VoteID detected: " << v.getVoteID() << endl;
            continue;
        }
        seenVoteIDs.insert(v.getVoteID());
        cout << "VoteID: " << v.getVoteID()
             << " | VoterID: " << v.getVoterID()
             << " | CandidateID: " << v.getCandidateID()
             << " | ElectionID: " << v.getElectionID()
             << " | PollingStationID:" << v.getPollingStationID()
             << " | Time: " << v.getTimestamp() << endl;
    }
}

void manageVoting() {
    int choice;
    while (true) {
    
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> votingManagement = {
        "View All Votes",
        "Back"
    };

    int choice = ShowMenu(screen, "Voting Management", votingManagement);
        if (choice == 0) {
            listAllVotes();
        } else if (choice == 1) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}
// int main()
// {
//     // Example usage
//     Vote v1(getNextID("VoteID"), 103, 202, 303, 404, 999, getCurrentTimestamp());
//     castVote(v1);
//     listAllVotes();
//     return 0;
// }