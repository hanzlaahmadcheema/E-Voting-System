#include <custom/config.h>

#include <models/Voter.h>
#include <models/Candidate.h>
#include <models/PollingStation.h>
#include <models/Vote.h>

extern vector<Voter> loadAllVoters();
extern vector<Candidate> loadAllCandidates();
extern bool castVote(const Vote&);
extern bool voteExists(int VoterID, int ElectionID);
extern void listAllCandidates();
extern PollingStation getPollingStationByID(int id);
extern void viewCandidatesByConstituency(int constID);
extern void viewCandidatesByStation(int PollingStationID);
extern string getCurrentTimestamp();
extern string getElectionTypeByID(int id);
extern void viewCandidatesByType(string type);
extern int getNextID(const string &key);
int getElectionIDByConstituencyID(int id);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

auto screen = ScreenInteractive::TerminalOutput();

Voter* voterLogin() {
    string VoterCNIC;

    vector<InputField> fields = {
        {"CNIC", &VoterCNIC, InputField::TEXT}
    };
    bool success = ShowForm(screen, "Voter Login", fields);
    if (!success) {
        cout << "\n[ERROR] Login cancelled.\n";
        return nullptr;
    }

    vector<Voter> voters = loadAllVoters();
    if (VoterCNIC.empty()) {
        cout << "CNIC cannot be empty.\n";
        return nullptr;
    }
    for (auto& v : voters) {
        if (v.getVoterCNIC() == VoterCNIC) {
            cout << "Login successful. Welcome " << v.getVoterName() << "!\n";
            return new Voter(v);
        }
    }
    cout << "Voter not found.\n";
    return nullptr;
}

void showUserMenu(Voter* voter) {
    PollingStation ps = getPollingStationByID(voter->getPollingStationID());
    int c1 = ps.getConstituencyIDNA();
    int c2 = ps.getConstituencyIDPA();
    while (true) {

    vector<string> voterMenu = {
        "View Candidates",
        "Cast Vote",
        "Logout"
    };

    int choice = ShowMenu(screen, "Voter Menu", voterMenu);
        if (choice == 0) {
            cout << "Candidates in Constituency " << c1 << ":\n";
            viewCandidatesByConstituency(c1);
            cout << "Candidates in Constituency " << c2 << ":\n";
            viewCandidatesByConstituency(c2);
        } else if (choice == 1) {
            int ElectionID, CandidateID, chosenConst;
            cout << "You can vote in Constituency " << c1 << " or " << c2 << ".\n";
            cout << "Enter Constituency ID to vote in: ";
            cin >> chosenConst;
            if (chosenConst != c1 && chosenConst != c2) {
                cout << "Invalid Constituency.\n";
                continue;
            }
            ElectionID = getElectionIDByConstituencyID(chosenConst);
            if (ElectionID == -1) {
                cout << "No election found for this constituency.\n";
                continue;
            }
            if (voteExists(voter->getVoterID(), ElectionID)) {
                cout << "You’ve already voted in this election.\n";
                continue;
            }
            viewCandidatesByConstituency(chosenConst);
            cout << "Enter Candidate ID to vote for: ";
            cin >> CandidateID;
            Vote vote(getNextID("VoteID"), voter->getVoterID(), CandidateID, ElectionID, voter->getPollingStationID(), getCurrentTimestamp());
            castVote(vote);
        } else if (choice == 2) {
            cout << "Logged out.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void userPanel() {
    cout << "\nWelcome to E-Voting System\n";
    Voter* voter = voterLogin();
    if (voter != nullptr) {
        showUserMenu(voter);
        delete voter;
    }
}
