#include <custom/config.h>

// Forward declarations
extern int getNextID(const string &key);
extern int ShowMenu(ScreenInteractive & screen, const string& heading, const vector<string>& options);
void ShowTableFTXUI(const string& heading, const vector<string>& headers, const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);



const string VOTE_FILE = "D://E-Voting-System/data/votes.json";

// Vote implementation
Vote::Vote() : VoteID(0), VoterID(0), CandidateID(0), ElectionID(0), PollingStationID(0), VoteTime("") {}
Vote::Vote(int VoteID, int VoterID, int CandidateID, int ElectionID, int PollingStationID, const string &VoteTime)
    : VoteID(VoteID), VoterID(VoterID), CandidateID(CandidateID), ElectionID(ElectionID), PollingStationID(PollingStationID), VoteTime(VoteTime) {}

void Vote::setVoteID(int VoteID) { this->VoteID = VoteID; }
void Vote::setVoterID(int VoterID) { this->VoterID = VoterID; }
void Vote::setCandidateID(int CandidateID) { this->CandidateID = CandidateID; }
void Vote::setElectionID(int ElectionID) { this->ElectionID = ElectionID; }
void Vote::setPollingStationID(int PollingStationID) { this->PollingStationID = PollingStationID; }
void Vote::setTimestamp(const string &VoteTime) { this->VoteTime = VoteTime; }

int Vote::getVoteID() const { return VoteID; }
int Vote::getVoterID() const { return VoterID; }
int Vote::getCandidateID() const { return CandidateID; }
int Vote::getElectionID() const { return ElectionID; }
int Vote::getPollingStationID() const { return PollingStationID; }
string Vote::getTimestamp() const { return VoteTime; }

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

Vote Vote::fromJSON(const json &j) {
    try {
        return Vote(
            j.at("VoteID").get<int>(),
            j.at("VoterID").get<int>(),
            j.at("CandidateID").get<int>(),
            j.at("ElectionID").get<int>(),
            j.at("PollingStationID").get<int>(),
            j.at("VoteTime").get<string>()
        );
    } catch (const exception &e) {
        cerr << "Error parsing Vote from JSON: " << e.what() << endl;
        // Return an invalid Vote object
        return Vote();
    }
}

// Helper: Validate Vote fields
bool isValidVote(const Vote &vote, const vector<Vote> &existingVotes, string &errorMsg) {
    if (vote.getVoteID() <= 0) {
        errorMsg = "VoteID must be positive.";
        return false;
    }
    if (vote.getVoterID() <= 0) {
        errorMsg = "VoterID must be positive.";
        return false;
    }
    if (vote.getCandidateID() <= 0) {
        errorMsg = "CandidateID must be positive.";
        return false;
    }
    if (vote.getElectionID() <= 0) {
        errorMsg = "ElectionID must be positive.";
        return false;
    }
    if (vote.getPollingStationID() <= 0) {
        errorMsg = "PollingStationID must be positive.";
        return false;
    }
    // Check for duplicate VoteID
    for (const auto &v : existingVotes) {
        if (v.getVoteID() == vote.getVoteID()) {
            errorMsg = "Duplicate VoteID.";
            return false;
        }
    }
    // Check for duplicate vote by same voter in same election
    for (const auto &v : existingVotes) {
        if (v.getVoterID() == vote.getVoterID() && v.getElectionID() == vote.getElectionID()) {
            errorMsg = "You have already voted in this election.";
            return false;
        }
    }
    // Check timestamp format (basic ISO 8601 check)
    regex iso8601(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    if (!regex_search(vote.getTimestamp(), iso8601)) {
        errorMsg = "Invalid timestamp format. Use YYYY-MM-DD HH:MM:SS";
        return false;
    }
    return true;
}

string getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm *tm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return string(buffer);
}

// Load all votes with error handling
vector<Vote> loadAllVotes() {
    vector<Vote> list;
    ifstream file(VOTE_FILE);
    if (!file.is_open()) {
        cerr << "Could not open vote file: " << VOTE_FILE << endl;
        return list;
    }
    try {
        json j;
        file >> j;
        for (auto &obj : j) {
            Vote v = Vote::fromJSON(obj);
            // Only add valid votes
            string err;
            if (isValidVote(v, list, err))
                list.push_back(v);
        }
    } catch (const exception &e) {
        cerr << "Error loading votes: " << e.what() << endl;
    }
    return list;
}

// Save all votes with error handling
bool saveAllVotes(const vector<Vote> &list) {
    ofstream file(VOTE_FILE);
    if (!file.is_open()) {
        cerr << "Could not open vote file for writing: " << VOTE_FILE << endl;
        return false;
    }
    try {
        json j;
        for (const auto &v : list) {
            j.push_back(v.toJSON());
        }
        file << j.dump(4);
        return true;
    } catch (const exception &e) {
        cerr << "Error saving votes: " << e.what() << endl;
        return false;
    }
}

bool voteExists(int VoterID, int ElectionID) {
    vector<Vote> list = loadAllVotes();
    unordered_map<int, unordered_set<int>> voterElectionMap;
    for (const auto& v : list) {
        voterElectionMap[v.getVoterID()].insert(v.getElectionID());
    }
    auto it = voterElectionMap.find(VoterID);
    if (it != voterElectionMap.end()) {
        if (it->second.count(ElectionID)) {
            return true;
        }
    }
    return false;
}

// User: Cast vote with error handling
bool castVote(const Vote &newVote) {
    vector<Vote> votes = loadAllVotes();
    string errorMsg;
    if (!isValidVote(newVote, votes, errorMsg)) {
        ShowMessage(screen, "Vote not cast: " + errorMsg, "Error");
        return false;
    }
    votes.push_back(newVote);
    if (!saveAllVotes(votes)) {
        ShowMessage(screen, "Failed to save vote to file.", "Error");
        return false;
    }
    ShowMessage(screen, "Vote cast successfully.", "Success");
    return true;
}

void deleteVotesByVoterID(int VoterID) {
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [VoterID](const Vote &v) { return v.getVoterID() == VoterID; });
    votes.erase(it, votes.end());
    if (!saveAllVotes(votes)) {
        ShowMessage(screen, "Failed to save changes to file.", "Error");
        return;
    }
    size_t after = votes.size();
    if (after < before)
        ShowMessage(screen, "Votes deleted for Voter ID: " + to_string(VoterID), "Success");
    else
        ShowMessage(screen, "No votes found for Voter ID: " + to_string(VoterID), "Info");
}

void deleteVotesByCandidateID(int CandidateID) {
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [CandidateID](const Vote &v) { return v.getCandidateID() == CandidateID; });
    votes.erase(it, votes.end());
    if (!saveAllVotes(votes)) {
        ShowMessage(screen, "Failed to save changes to file.", "Error");
        return;
    }
    size_t after = votes.size();
    if (after < before)
        ShowMessage(screen, "Votes deleted for Candidate ID: " + to_string(CandidateID), "Success");
    else
        ShowMessage(screen, "No votes found for Candidate ID: " + to_string(CandidateID), "Info");
}

void deleteVotesByElectionID(int ElectionID) {
    vector<Vote> votes = loadAllVotes();
    size_t before = votes.size();
    auto it = remove_if(votes.begin(), votes.end(), [ElectionID](const Vote &v) { return v.getElectionID() == ElectionID; });
    votes.erase(it, votes.end());
    if (!saveAllVotes(votes)) {
        ShowMessage(screen, "Failed to save changes to file.", "Error");
        return;
    }
    size_t after = votes.size();
    if (after < before)
        ShowMessage(screen, "Votes deleted for Election ID: " + to_string(ElectionID), "Success");
    else
        ShowMessage(screen, "No votes found for Election ID: " + to_string(ElectionID), "Info");
}

// Admin: View all votes with validation
void listAllVotes() {
    vector<Vote> votes = loadAllVotes();
    set<int> seenVoteIDs;
    vector<vector<string>> rows;
    vector<string> headers = {"VoteID", "VoterID", "CandidateID", "ElectionID", "PollingStationID", "Timestamp"};
    for (const auto &v : votes) {
        string errorMsg;
        if (!isValidVote(v, {}, errorMsg)) {
            ShowMessage(screen, "Invalid vote (VoteID: " + to_string(v.getVoteID()) + " ): " + errorMsg, "Error");
            continue;
        }
        if (seenVoteIDs.count(v.getVoteID())) {
            ShowMessage(screen, "Duplicate VoteID detected: " + to_string(v.getVoteID()), "Error");
            continue;
        }
        seenVoteIDs.insert(v.getVoteID());
        rows.push_back({
            to_string(v.getVoteID()),
            to_string(v.getVoterID()),
            to_string(v.getCandidateID()),
            to_string(v.getElectionID()),
            to_string(v.getPollingStationID()),
            v.getTimestamp()
        });
    }
    ShowTableFTXUI("Vote Details", headers, rows);
}

void manageVoting() {
           ShowSpinner(screen, "Loading Voting Management...");
    while (true) {
        vector<string> votingManagement = {
            "View All Votes",
            "Back"
        };
        int choice = ShowMenu(screen, "Voting Management", votingManagement);
        if (choice == 0) {
            listAllVotes();
        } else if (choice == 1) {
            break;
        } else {
            ShowMessage(screen, "Invalid option. Please try again.", "Error");
        }
    }
}
