#include <iostream>
using namespace std;

void manageElections();
void manageCities();
void manageConstituencies();
void managePollingStations();
void manageParties();
void manageCandidates();
void manageVoters();
void manageVoting();
void manageResults();

void adminPanel() {
    int choice;
    while (true) {
        cout << "\n [ADMIN PANEL]\n";
        cout << "1. Elections\n";
        cout << "2. Cities\n";
        cout << "3. Constituencies\n";
        cout << "4. Polling Stations\n";
        cout << "5. Political Parties\n";
        cout << "6. Candidates\n";
        cout << "7. Voters\n";
        cout << "8. Voting Process\n";
        cout << "9. Results\n";
        cout << "0. Exit Admin Panel\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1: manageElections(); break;
            case 2: manageCities(); break;
            case 3: manageConstituencies(); break;
            case 4: managePollingStations(); break;
            case 5: manageParties(); break;
            case 6: manageCandidates(); break;
            case 7: manageVoters(); break;
            case 8: manageVoting(); break;
            case 9: manageResults(); break;
            case 0: return;
            default: cout << "❌ Invalid choice. Try again.\n";
        }
    }
}
