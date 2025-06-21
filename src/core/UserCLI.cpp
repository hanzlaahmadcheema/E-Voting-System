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

bool isValidCNIC(const string& cnic);

bool isValidNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

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

    if (VoterCNIC.empty()) {
       ShowMessage(screen,"CNIC cannot be empty.", "error");
       return nullptr;
    }
    if (!isValidCNIC(VoterCNIC)) {
       ShowMessage(screen,"Invalid CNIC format. Must be 13 digits.", "error");
       return nullptr;
    }

    vector<Voter> voters;
    try {
       voters = loadAllVoters();
    } catch (const std::exception& e) {
       ShowMessage(screen, string("Error loading voters: ") + e.what(), "error");
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
    PollingStation ps;
    try {
       ps = getPollingStationByID(voter->getPollingStationID());
    } catch (const std::exception& e) {
       ShowMessage(screen, string("Error loading polling station: ") + e.what(), "error");
       return;
    }
    int c1 = ps.getConstituencyIDNA();
    int c2 = ps.getConstituencyIDPA();
    while (true) {
       vector<string> voterMenu = {
          "View Candidates",
          "Cast Vote",
          "Logout"
       };

       int choice = -1;
       try {
          choice = ShowMenu(screen, "Voter Menu", voterMenu);
       } catch (const std::exception& e) {
          ShowMessage(screen, string("Menu error: ") + e.what(), "error");
          continue;
       }

       if (choice == 0) {
          ShowMessage(screen,"Candidates in Constituency " + to_string(c1) + ":", "info");
          try {
             viewCandidatesByConstituency(c1);
             ShowMessage(screen,"Candidates in Constituency " + to_string(c2) + ":", "info");
             viewCandidatesByConstituency(c2);
          } catch (const std::exception& e) {
             ShowMessage(screen, string("Error viewing candidates: ") + e.what(), "error");
          }
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
          if (!isValidNumber(chosenConst)) {
             ShowMessage(screen,"Invalid Constituency ID format.", "error");
             continue;
          }
          int chosenConstID = stoi(chosenConst);
          if (chosenConstID != c1 && chosenConstID != c2) {
             ShowMessage(screen,"Invalid Constituency ID. Please choose " + to_string(c1) + " or " + to_string(c2) + ".", "error");
             continue;
          }
          try {
             ElectionID = getElectionIDByConstituencyID(chosenConstID);
          } catch (const std::exception& e) {
             ShowMessage(screen, string("Error getting election: ") + e.what(), "error");
             continue;
          }
          if (ElectionID == -1) {
             ShowMessage(screen,"No election found for this constituency.", "info");
             continue;
          }
          try {
             if (voteExists(voter->getVoterID(), ElectionID)) {
                ShowMessage(screen,"You’ve already voted in this election.", "info");
                continue;
             }
          } catch (const std::exception& e) {
             ShowMessage(screen, string("Error checking vote: ") + e.what(), "error");
             continue;
          }
          try {
             viewCandidatesByConstituency(chosenConstID);
          } catch (const std::exception& e) {
             ShowMessage(screen, string("Error viewing candidates: ") + e.what(), "error");
             continue;
          }
          vector<InputField> candidateForm = {
             {"Candidate ID", &CandidateID, InputField::NUMBER}
          };
          success = ShowForm(screen, "Cast Vote", candidateForm);
          if (!success) {
             ShowMessage(screen,"Form submission cancelled.", "error");
             continue;
          }
          if (!isValidNumber(CandidateID)) {
             ShowMessage(screen,"Invalid Candidate ID format.", "error");
             continue;
          }
          int CandidateIDInt = stoi(CandidateID);
          try {
             Vote vote(getNextID("VoteID"), voter->getVoterID(), CandidateIDInt, ElectionID, voter->getPollingStationID(), getCurrentTimestamp());
             if (!castVote(vote)) {
                ShowMessage(screen,"Failed to cast vote. Please try again.", "error");
             } else {
                ShowMessage(screen,"Vote cast successfully!", "success");
             }
          } catch (const std::exception& e) {
             ShowMessage(screen, string("Error casting vote: ") + e.what(), "error");
          }
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
    Voter* voter = nullptr;
        while (true) {
    try {
       voter = voterLogin();
    } catch (const std::exception& e) {
       ShowMessage(screen, string("Login error: ") + e.what(), "error");
       return;
    }
    if (voter != nullptr) {
       try {
          showUserMenu(voter);
       } catch (const std::exception& e) {
          ShowMessage(screen, string("Error in user menu: ") + e.what(), "error");
       }
       delete voter;
   break; // Exit after successful login and menu
        } else {
            // Optionally, ask if the user wants to try again or exit
            continue;
        }
      }
}
