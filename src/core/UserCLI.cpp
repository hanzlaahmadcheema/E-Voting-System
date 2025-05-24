#include <iostream>
#include <ctime>
#include "../models/Voter.h"
#include "../models/Vote.h"
#include "../models/Candidate.h"
#include "../models/PollingStation.h"


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

using namespace std;


Voter* voterLogin() {
    string VoterCNIC;
    cout << "Enter CNIC to log in: ";
    getline(cin, VoterCNIC);

    vector<Voter> voters = loadAllVoters();
    if (VoterCNIC.empty()) {
        cout << "CNIC cannot be empty.\n";
        return nullptr;
    }
    for (auto& v : voters) {
        if (v.getVoterCNIC() == VoterCNIC) {
            cout << "Login successful. Welcome " << v.getVoterName() << "!\n";
            return new Voter(v);  // Create a copy
        }
    }
    cout << "Voter not found.\n";
    return nullptr;
}

void showUserMenu(Voter* voter) {
    int choice;
    PollingStation ps = getPollingStationByID(voter->getPollingStationID());
    int c1 = ps.getConstituencyID1();
    int c2 = ps.getConstituencyID2();
    while (true) {
        cout << "\n Voter Menu\n";
        cout << "1. View Candidates\n";
        cout << "2. Cast Vote\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Candidates in Constituency " << c1 << ":\n";
            viewCandidatesByConstituency(c1);
            cout << "Candidates in Constituency " << c2 << ":\n";
            viewCandidatesByConstituency(c2);
        } else if (choice == 2) {
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
        } else if (choice == 0) {
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
