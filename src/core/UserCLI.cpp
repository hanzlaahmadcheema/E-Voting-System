#include <iostream>
#include <ctime>
#include "../models/Voter.h"
#include "../models/Vote.h"
#include "../models/Candidate.h"


extern vector<Voter> loadAllVoters();
extern vector<Candidate> loadAllCandidates();
extern bool castVote(const Vote&);
extern bool voteExists(int VoterID, int ElectionID);
extern void listAllCandidates();
extern void viewCandidatesByStation(int PollingStationID);
extern string getCurrentTimestamp();
extern string getElectionTypeByID(int id);
extern void viewCandidatesByType(string type);
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
    while (true) {
        cout << "\n Voter Menu\n";
        cout << "1. View Candidates\n";
        cout << "2. Cast Vote\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            viewCandidatesByStation(voter->getPollingStationID());
        } else if (choice == 2) {
            int ElectionID, CandidateID, PollingStationID;
            string type;
            cout << "Enter Election ID: ";
            cin >> ElectionID;

            if (voteExists(voter->getVoterID(), ElectionID)) {
                cout << "You’ve already voted in this election.\n";
                continue;
            }
            type = getElectionTypeByID(ElectionID);

            viewCandidatesByType(type);

            cout << "Enter Candidate ID to vote for: ";
            cin >> CandidateID;
            cout << "Casting vote....\n ";
            Vote vote(1, voter->getVoterID(), CandidateID, ElectionID, voter->getPollingStationID(), getCurrentTimestamp());
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
