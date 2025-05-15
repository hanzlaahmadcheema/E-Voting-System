#include "Result.h"
#include "Vote.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<Vote> loadAllVotes();

//Result
Result::Result() : ResultID(0), PollingStationID(0), ElectionID(0), WinnerCandidateID(0), TotalVotes(0), ConstituencyID(0) {}
Result::Result(int ResultID, int PollingStationID, int ElectionID, int WinnerCandidateID, int TotalVotes, int ConstituencyID) {
    this->ResultID = ResultID;
    this->PollingStationID = PollingStationID;
    this->ElectionID = ElectionID;
    this->WinnerCandidateID = WinnerCandidateID;
    this->TotalVotes = TotalVotes;
    this->ConstituencyID = ConstituencyID;
    
}
void Result::declareResult() {
    cout << "Result declared for Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << WinnerCandidateID << "\n"
              << "Total Votes: " << TotalVotes << endl;
}
void Result::setResultID(int ResultID) {
    this->ResultID = ResultID;
}
void Result::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Result::setElectionID(int ElectionID) {
    this->ElectionID = ElectionID;
}
void Result::setWinnerCandidateID(int WinnerCandidateID) {
    this->WinnerCandidateID = WinnerCandidateID;
}
void Result::setTotalVotes(int TotalVotes) {
    this->TotalVotes = TotalVotes;
}
void Result::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int Result::getConstituencyID() const {
    return ConstituencyID;
}
int Result::getResultID() const {
    return ResultID;
}
int Result::getPollingStationID() const {
    return PollingStationID;
}
int Result::getElectionID() const {
    return ElectionID;
}
int Result::getWinnerCandidateID() const {
    return WinnerCandidateID;
}
int Result::getTotalVotes() const {
    return TotalVotes;
}
void Result::displayResultInfo() const {
    cout << "Result ID: " << ResultID << "\n"
              << "Polling Station ID: " << PollingStationID << "\n"
              << "Election ID: " << ElectionID << "\n"
              << "Winner Candidate ID: " << WinnerCandidateID << "\n"
              << "Total Votes: " << TotalVotes << endl;
}

json Result::toJSON() const {
    return json{
        {"ResultID", ResultID},
        {"PollingStationID", PollingStationID},
        {"ElectionID", ElectionID},
        {"WinnerCandidateID", WinnerCandidateID},
        {"TotalVotes", TotalVotes},
        {"ConstituencyID", ConstituencyID}
       
    };
}

Result Result::fromJSON(const json& j) {
    return Result(
        j.at("ResultID").get<int>(),
        j.at("PollingStationID").get<int>(),
        j.at("ElectionID").get<int>(),
        j.at("WinnerCandidateID").get<int>(),
        j.at("TotalVotes").get<int>(),
        j.at("ConstituencyID").get<int>()
        
    );
}

const string RESULT_FILE = "../../data/results.json";
const string VOTE_FILE = "../../data/votes.json";

// Load all results
vector<Result> loadAllResults() {
    vector<Result> results;
    ifstream file(RESULT_FILE);
    if (file.is_open()) {
        json j;
        file >> j;
        for (auto& obj : j) {
            results.push_back(Result::fromJSON(obj));
        }
    }
    return results;
}

// Save all results
void saveAllResults(const vector<Result>& results) {
    ofstream file(RESULT_FILE);
    json j;
    for (const auto& r : results) {
        j.push_back(r.toJSON());
    }
    file << j.dump(4);
}

// Admin: Compute results for a constituency in an election
void computeConstituencyResult(int electionID, int constituencyID) {
    vector<Vote> votes = loadAllVotes();
    unordered_map<int, int> voteCounts; // CandidateID -> Vote count

    for (const auto& vote : votes) {
        if (vote.getElectionID() == electionID) {
            // Assume candidate IDs are filtered by constituency outside this logic
            voteCounts[vote.getCandidateID()]++;
        }
    }

    // Find the candidate with the most votes
    int maxVotes = 0;
    int winnerCandidateID = -1;
    for (const auto& pair : voteCounts) {
        if (pair.second > maxVotes) {
            maxVotes = pair.second;
            winnerCandidateID = pair.first;
        }
    }

    // Save result
    Result result(0, constituencyID, electionID, winnerCandidateID, maxVotes, constituencyID);
    vector<Result> allResults = loadAllResults();
    allResults.push_back(result);
    saveAllResults(allResults);

    cout << "✅ Result computed for Constituency " << constituencyID
         << " | Winner CandidateID: " << winnerCandidateID
         << " with " << maxVotes << " votes.\n";
}

// Admin/User: View result for a constituency
void viewResultByConstituency(int electionID, int constituencyID) {
    vector<Result> results = loadAllResults();
    for (const auto& r : results) {
        if (r.getElectionID() == electionID && r.getConstituencyID() == constituencyID) {
            cout << "📊 Constituency " << constituencyID << " | Winner: CandidateID "
                 << r.getWinnerCandidateID() << " | Total Votes: " << r.getTotalVotes() << endl;
            return;
        }
    }
    cout << "⚠️ No result found for this constituency.\n";
}

// Admin/User: List all results
void listAllResults() {
    vector<Result> results = loadAllResults();
    for (const auto& r : results) {
        cout << "📌 ElectionID: " << r.getElectionID()
             << " | ConstID: " << r.getConstituencyID()
             << " | WinnerID: " << r.getWinnerCandidateID()
             << " | Votes: " << r.getTotalVotes() << endl;
    }
}