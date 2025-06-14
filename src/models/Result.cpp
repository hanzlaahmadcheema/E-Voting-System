#include <custom/config.h>




extern vector<Vote> loadAllVotes();
extern int getNextID(const string &key);
extern string getConstituencyTypeByID(int id);
extern vector<PollingStation> loadAllStations();
extern string getCandidateNameByID(int CandidateID);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

;

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
            catch (const exception &e)
            {
                cerr << "Warning: Skipping invalid result entry: " << e.what() << endl;
            }
        }
    }
    catch (const exception &e)
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
        catch (const exception &e)
        {
            cerr << "Warning: Could not serialize result: " << e.what() << endl;
        }
    }
    file << j.dump(4);
}

// Admin: Compute results for a constituency in an election
void computeConstituencyResult(int ElectionID, int ConstituencyID, const string& constituencyCode)
{
    if (ElectionID <= 0 || ConstituencyID <= 0 || constituencyCode.empty())
    {
        cerr << "Error: Invalid ElectionID, ConstituencyID, or constituencyCode." << endl;
        return;
    }

    // Determine constituency type from code
    string constituencyType;
    if (constituencyCode.rfind("NA", 0) == 0) {
        constituencyType = "NA";
    } else if (constituencyCode.rfind("P", 0) == 0) {
        constituencyType = "PA";
    } else {
        cerr << "Error: Unknown constituency code format." << endl;
        return;
    }

    vector<PollingStation> stations = loadAllStations();
    set<int> relevantStationIDs;
    for (const auto& ps : stations) {
        if ((constituencyType == "NA" && ps.getConstituencyIDNA() == ConstituencyID) ||
            (constituencyType == "PA" && ps.getConstituencyIDPA() == ConstituencyID)) {
            relevantStationIDs.insert(ps.getPollingStationID());
        }
    }
    if (relevantStationIDs.empty()) {
        cerr << "Error: No polling stations found for this constituency." << endl;
        return;
    }

    vector<Vote> votes = loadAllVotes();
    unordered_map<int, int> voteCounts; // CandidateID -> Vote count

    for (const auto& vote : votes) {
        if (vote.getElectionID() == ElectionID &&
            relevantStationIDs.count(vote.getPollingStationID()) > 0) {
            int CandidateID = vote.getCandidateID();
            if (CandidateID > 0)
                voteCounts[CandidateID]++;
        }
    }

    if (voteCounts.empty()) {
        cerr << "Error: No votes found for this election and constituency." << endl;
        return;
    }

    // Find the candidate with the most votes
    int maxVotes = 0;
    int winnerCandidateID = -1;
    bool tie = false;
    for (const auto& pair : voteCounts) {
        if (pair.second > maxVotes) {
            maxVotes = pair.second;
            winnerCandidateID = pair.first;
            tie = false;
        } else if (pair.second == maxVotes) {
            tie = true;
        }
    }

    if (tie) {
        cerr << "Warning: There is a tie between candidates for this constituency." << endl;
        // Optionally, handle tie-break logic here.
    }

    // Check for duplicate result
    vector<Result> allResults = loadAllResults();
    for (const auto& r : allResults) {
        if (r.getElectionID() == ElectionID && r.getConstituencyID() == ConstituencyID) {
            cerr << "Error: Result for this election and constituency already exists." << endl;
            return;
        }
    }

    Result result(getNextID("ResultID"), 0, ElectionID, winnerCandidateID, maxVotes, ConstituencyID);
    allResults.push_back(result);
    saveAllResults(allResults);
    

    vector<string> headers = {"Result ID", "Polling Station ID", "Election ID", "Winner Candidate", "Total Votes", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &r : allResults)
    {
        data.push_back({
            to_string(r.getResultID()),
            to_string(r.getPollingStationID()),
            to_string(r.getElectionID()),
            getCandidateNameByID(r.getWinnerCandidateID()),
            to_string(r.getTotalVotes()),
            to_string(r.getConstituencyID())
        });
    }
    ShowTableFTXUI("Results In Constituency" + to_string(ConstituencyID), headers, data);
    

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
                    vector<string> headers = {"Result ID", "Polling Station ID", "Election ID", "Winner Candidate", "Total Votes", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &r : results)
    {
        if (r.getElectionID() == ElectionID && r.getConstituencyID() == ConstituencyID)
        {
        data.push_back({
            to_string(r.getResultID()),
            to_string(r.getPollingStationID()),
            to_string(r.getElectionID()),
            getCandidateNameByID(r.getWinnerCandidateID()),
            to_string(r.getTotalVotes()),
            to_string(r.getConstituencyID())
        });
            return;
        }
    }
      ShowMessage(screen, "No result found for this constituency.", "Error");
}

// void CleanOrphanedVotes(vector<Vote>& votes, const vector<Voter>& voters, const vector<Candidate>& candidates) {
//     set<int> validVoterIDs, validCandidateIDs;
//     for (const auto& v : voters) validVoterIDs.insert(v.getVoterID());
//     for (const auto& c : candidates) validCandidateIDs.insert(c.getCandidateID());

//     for (auto it = votes.begin(); it != votes.end(); ) {
//         if (validVoterIDs.count(it->getVoterID()) == 0 || validCandidateIDs.count(it->getCandidateID()) == 0) {
//             it = votes.erase(it);
//         } else {
//             ++it;
//         }
//     }
// }

// Admin/User: List all results
void listAllResults()
{
    vector<Result> results = loadAllResults();
    if (results.empty())
    {
          ShowMessage(screen,"No results to display.","Error");
        return;
    }
    
        vector<string> headers = {"Result ID", "Polling Station ID", "Election ID", "Winner Candidate", "Total Votes", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &r : results)
    {
        data.push_back({
            to_string(r.getResultID()),
            to_string(r.getPollingStationID()),
            to_string(r.getElectionID()),
            getCandidateNameByID(r.getWinnerCandidateID()),
            to_string(r.getTotalVotes()),
            to_string(r.getConstituencyID())
        });
    }
    ShowTableFTXUI("All Results", headers, data);
}

void manageResults() {
    int choice;
    while (true) {
   

    vector<string> resultsManagement = {
        "Compute Result by Constituency",
        "View Result by Constituency",
        "View All Results",
        "Back"
    };

    int choice = ShowMenu(screen, "Results Management", resultsManagement);
        if (choice == 0) {
            string ElectionID_str, constID_str;
    
            vector<InputField> form = {
                {"Election ID", &ElectionID_str, InputField::NUMBER},
                {"Constituency ID", &constID_str, InputField::NUMBER}
            };
            bool success = ShowForm(screen, "Compute Result", form);
            if (!success) {
                  ShowMessage(screen,"[ERROR] Result computation cancelled.","error");
                continue;
            }
            int ElectionID = stoi(ElectionID_str);
            int constID = stoi(constID_str);
            computeConstituencyResult(ElectionID, constID, getConstituencyTypeByID(constID));
        } else if (choice == 1) {
            string ElectionID_str, constID_str;
    
            vector<InputField> form = {
                {"Election ID", &ElectionID_str, InputField::NUMBER},
                {"Constituency ID", &constID_str, InputField::NUMBER}
            };
            bool success = ShowForm(screen, "Compute Result", form);
            if (!success) {
                  ShowMessage(screen,"[ERROR] Result computation cancelled.","error");
                continue;
            }
            int ElectionID = stoi(ElectionID_str);
            int constID = stoi(constID_str);
            viewResultByConstituency(ElectionID, constID);
        } else if (choice == 2) {
            listAllResults();
        } else if (choice == 3) {
            break;
        } else {
              ShowMessage(screen," Invalid option. Please try again.","error");
        }
    }
}

// int main()
// {
//     // Example usage
//     Result r1(getNextID("ResultID"), 101, 2023, 5, 1000, 1);
//     r1.declareResult();
//     r1.displayResultInfo();
//     computeConstituencyResult(2023, r1.getConstituencyID());
//     viewResultByConstituency(2023, r1.getConstituencyID());
//     listAllResults();

//     return 0;
// }