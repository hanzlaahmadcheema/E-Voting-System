#include <custom/config.h>

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


Voter* voterLogin() {
    string VoterCNIC;


    vector<InputField> fields = {
        {"CNIC", &VoterCNIC, InputField::TEXT}
    };
    bool success = ShowForm(screen, "Voter Login", fields);
    if (!success) {
        ShowMessage(screen,"Login cancelled.", "error");
        return nullptr;
    }

    vector<Voter> voters = loadAllVoters();
    if (VoterCNIC.empty()) {
        ShowMessage(screen,"CNIC cannot be empty.", "error");
        return nullptr;
    }
    for (auto& v : voters) {
        if (v.getVoterCNIC() == VoterCNIC) {
            ShowMessage(screen,"Login successful. Welcome " + v.getVoterName() + "!", "success");
            return new Voter(v);
        }
    }
    ShowMessage(screen,"Voter not found.", "info");
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
            ShowMessage(screen,"Candidates in Constituency " + to_string(c1) + ":", "info");
            viewCandidatesByConstituency(c1);
            ShowMessage(screen,"Candidates in Constituency " + to_string(c2) + ":", "info");
            viewCandidatesByConstituency(c2);
        } else if (choice == 1) {
            int ElectionID;
            string CandidateID, chosenConst;
            ShowMessage(screen,"You can vote in Constituency " + to_string(c1) + " or " + to_string(c2) + ".", "info");
            vector<InputField> form = {
                {"Constituency ID", &chosenConst, InputField::NUMBER}
            };
            bool success = ShowForm(screen, "Cast Vote", form);
            if (!success) {
                ShowMessage(screen,"Form submission cancelled.", "error");
                continue;
            }
            int chosenConstID = stoi(chosenConst);
            if (chosenConstID != c1 && chosenConstID != c2) {
                ShowMessage(screen,"Invalid Constituency ID. Please choose " + to_string(c1) + " or " + to_string(c2) + ".", "error");
                continue;
            }
            ElectionID = getElectionIDByConstituencyID(chosenConstID);
            if (ElectionID == -1) {
                ShowMessage(screen,"No election found for this constituency.", "info");
                continue;
            }
            if (voteExists(voter->getVoterID(), ElectionID)) {
                ShowMessage(screen,"You’ve already voted in this election.", "info");
                continue;
            }
            viewCandidatesByConstituency(chosenConstID);
            vector<InputField> candidateForm = {
                {"Candidate ID", &CandidateID, InputField::NUMBER}
            };
            success = ShowForm(screen, "Cast Vote", candidateForm);
            if (!success) {
                ShowMessage(screen,"Form submission cancelled.", "error");
                continue;
            }
            int CandidateIDInt = stoi(CandidateID);
            Vote vote(getNextID("VoteID"), voter->getVoterID(), CandidateIDInt, ElectionID, voter->getPollingStationID(), getCurrentTimestamp());
            castVote(vote);
        } else if (choice == 2) {
            ShowMessage(screen,"Logged out.", "success");
            break;
        } else {
            ShowMessage(screen,"Invalid choice.", "error");
        }
    }
}

void userPanel() {
    ShowMessage(screen,"Welcome to E-Voting System", "info");
    Voter* voter = voterLogin();
    if (voter != nullptr) {
        showUserMenu(voter);
        delete voter;
    }
}
