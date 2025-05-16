#include "Result.h"
#include "Vote.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<Vote> loadAllVotes();

// Result
Result::Result() : ResultID(0), PollingStationID(0), ElectionID(0), WinnerCandidateID(0), TotalVotes(0), ConstituencyID(0) {}
Result::Result(int ResultID, int PollingStationID, int ElectionID, int WinnerCandidateID, int TotalVotes, int ConstituencyID)
{
    this->ResultID = ResultID;
    this->PollingStationID = PollingStationID;
    this->ElectionID = ElectionID;
    this->WinnerCandidateID = WinnerCandidateID;
    this->TotalVotes = TotalVotes;
    this->ConstituencyID = ConstituencyID;
}
void Result::declareResult()
{
    cout << "Result declared for Election ID: " << ElectionID << "\n"
         << "Winner Candidate ID: " << WinnerCandidateID << "\n"
         << "Total Votes: " << TotalVotes << endl;
}
void Result::setResultID(int ResultID)
{
    this->ResultID = ResultID;
}
void Result::setPollingStationID(int PollingStationID)
{
    this->PollingStationID = PollingStationID;
}
void Result::setElectionID(int ElectionID)
{
    this->ElectionID = ElectionID;
}
void Result::setWinnerCandidateID(int WinnerCandidateID)
{
    this->WinnerCandidateID = WinnerCandidateID;
}
void Result::setTotalVotes(int TotalVotes)
{
    this->TotalVotes = TotalVotes;
}
void Result::setConstituencyID(int ConstituencyID)
{
    this->ConstituencyID = ConstituencyID;
}
int Result::getConstituencyID() const
{
    return ConstituencyID;
}
int Result::getResultID() const
{
    return ResultID;
}
int Result::getPollingStationID() const
{
    return PollingStationID;
}
int Result::getElectionID() const
{
    return ElectionID;
}
int Result::getWinnerCandidateID() const
{
    return WinnerCandidateID;
}
int Result::getTotalVotes() const
{
    return TotalVotes;
}
void Result::displayResultInfo() const
{
    cout << "Result ID: " << ResultID << "\n"
         << "Polling Station ID: " << PollingStationID << "\n"
         << "Election ID: " << ElectionID << "\n"
         << "Winner Candidate ID: " << WinnerCandidateID << "\n"
         << "Total Votes: " << TotalVotes << endl;
}

json Result::toJSON() const
{
    return json{
        {"ResultID", ResultID},
        {"PollingStationID", PollingStationID},
        {"ElectionID", ElectionID},
        {"WinnerCandidateID", WinnerCandidateID},
        {"TotalVotes", TotalVotes},
        {"ConstituencyID", ConstituencyID}

    };
}

Result Result::fromJSON(const json &j)
{
    return Result(
        j.at("ResultID").get<int>(),
        j.at("PollingStationID").get<int>(),
        j.at("ElectionID").get<int>(),
        j.at("WinnerCandidateID").get<int>(),
        j.at("TotalVotes").get<int>(),
        j.at("ConstituencyID").get<int>()

    );
}

const string RESULT_FILE = "data/results.json";
const string VOTE_FILE = "data/votes.json";

// Load all results
#include <sys/stat.h>

vector<Result> loadAllResults()
{
    vector<Result> results;
    struct stat buffer;
    if (stat(RESULT_FILE.c_str(), &buffer) != 0)
    {
        // File does not exist, create an empty JSON array file
        ofstream createFile(RESULT_FILE);
        if (createFile.is_open())
        {
            createFile << "[]";
            createFile.close();
        }
        else
        {
            cerr << "Error: Could not create results file: " << RESULT_FILE << endl;
            return results;
        }
    }
    ifstream file(RESULT_FILE);
    if (!file.is_open())
    {
        cerr << "Error: Could not open results file: " << RESULT_FILE << endl;
        return results;
    }
    try
    {
        json j;
        file >> j;
        if (!j.is_array())
        {
            cerr << "Error: Results file is not a valid JSON array." << endl;
            return results;
        }
        for (auto &obj : j)
        {
            try
            {
                results.push_back(Result::fromJSON(obj));
            }
            catch (const std::exception &e)
            {
                cerr << "Warning: Skipping invalid result entry: " << e.what() << endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        cerr << "Error: Failed to parse results file: " << e.what() << endl;
    }
    return results;
}

// Save all results
void saveAllResults(const vector<Result> &results)
{
    ofstream file(RESULT_FILE);
    if (!file.is_open())
    {
        cerr << "Error: Could not open results file for writing: " << RESULT_FILE << endl;
        return;
    }
    json j = json::array();
    for (const auto &r : results)
    {
        try
        {
            j.push_back(r.toJSON());
        }
        catch (const std::exception &e)
        {
            cerr << "Warning: Could not serialize result: " << e.what() << endl;
        }
    }
    file << j.dump(4);
}

// Admin: Compute results for a constituency in an election
void computeConstituencyResult(int ElectionID, int ConstituencyID)
{
    if (ElectionID <= 0 || ConstituencyID <= 0)
    {
        cerr << "Error: Invalid ElectionID or ConstituencyID." << endl;
        return;
    }
    vector<Vote> votes = loadAllVotes();
    if (votes.empty())
    {
        cerr << "Error: No votes found." << endl;
        return;
    }
    unordered_map<int, int> voteCounts; // CandidateID -> Vote count

    for (const auto &vote : votes)
    {
        if (vote.getElectionID() == ElectionID && vote.getConstituencyID() == ConstituencyID)
        {
            int CandidateID = vote.getCandidateID();
            if (CandidateID > 0)
                voteCounts[CandidateID]++;
        }
    }

    if (voteCounts.empty())
    {
        cerr << "Error: No votes found for this election and constituency." << endl;
        return;
    }

    // Find the candidate with the most votes
    int maxVotes = 0;
    int winnerCandidateID = -1;
    bool tie = false;
    for (const auto &pair : voteCounts)
    {
        if (pair.second > maxVotes)
        {
            maxVotes = pair.second;
            winnerCandidateID = pair.first;
            tie = false;
        }
        else if (pair.second == maxVotes)
        {
            tie = true;
        }
    }

    if (tie)
    {
        cerr << "Warning: There is a tie between candidates for this constituency." << endl;
        // Optionally, handle tie-break logic here.
    }

    // Check for duplicate result
    vector<Result> allResults = loadAllResults();
    for (const auto &r : allResults)
    {
        if (r.getElectionID() == ElectionID && r.getConstituencyID() == ConstituencyID)
        {
            cerr << "Error: Result for this election and constituency already exists." << endl;
            return;
        }
    }

    // Save result
    Result result(0, ConstituencyID, ElectionID, winnerCandidateID, maxVotes, ConstituencyID);
    allResults.push_back(result);
    saveAllResults(allResults);

    cout << "Result computed for Constituency " << ConstituencyID
         << " | Winner CandidateID: " << winnerCandidateID
         << " with " << maxVotes << " votes.\n";
}

// Admin/User: View result for a constituency
void viewResultByConstituency(int ElectionID, int ConstituencyID)
{
    if (ElectionID <= 0 || ConstituencyID <= 0)
    {
        cerr << "Error: Invalid ElectionID or ConstituencyID." << endl;
        return;
    }
    vector<Result> results = loadAllResults();
    if (results.empty())
    {
        cerr << "Error: No results found." << endl;
        return;
    }
    for (const auto &r : results)
    {
        if (r.getElectionID() == ElectionID && r.getConstituencyID() == ConstituencyID)
        {
            cout << "Constituency " << ConstituencyID << " | Winner: CandidateID "
                 << r.getWinnerCandidateID() << " | Total Votes: " << r.getTotalVotes() << endl;
            return;
        }
    }
    cout << "No result found for this constituency.\n";
}

// Admin/User: List all results
void listAllResults()
{
    vector<Result> results = loadAllResults();
    if (results.empty())
    {
        cout << "No results to display." << endl;
        return;
    }
    for (const auto &r : results)
    {
        cout << "ElectionID: " << r.getElectionID()
             << " | ConstID: " << r.getConstituencyID()
             << " | WinnerID: " << r.getWinnerCandidateID()
             << " | Votes: " << r.getTotalVotes() << endl;
    }
}

void manageResults() {
    int choice;
    while (true) {
        cout << "\n Results Management\n";
        cout << "1. Compute Result by Constituency\n";
        cout << "2. View Result by Constituency\n";
        cout << "3. View All Results\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int ElectionID, constID;
            cout << "Enter Election ID: ";
            cin >> ElectionID;
            cout << "Enter Constituency ID: ";
            cin >> constID;
            computeConstituencyResult(ElectionID, constID);
        } else if (choice == 2) {
            int ElectionID, constID;
            cout << "Enter Election ID: ";
            cin >> ElectionID;
            cout << "Enter Constituency ID: ";
            cin >> constID;
            viewResultByConstituency(ElectionID, constID);
        } else if (choice == 3) {
            listAllResults();
        } else if (choice == 0) {
            break;
        } else {
            cout << " Invalid option.\n";
        }
    }
}

// int main()
// {
//     // Example usage
//     Result r1(1, 101, 2023, 5, 1000, 1);
//     r1.declareResult();
//     r1.displayResultInfo();
//     computeConstituencyResult(2023, 1);
//     viewResultByConstituency(2023, 1);
//     listAllResults();

//     return 0;
// }