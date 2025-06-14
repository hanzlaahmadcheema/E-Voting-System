#include <custom/config.h>


const string ADMIN_FILE = "data/admins.json";
        extern void manageElections();
        extern void manageCities();
        extern void manageConstituencies();
        extern void managePollingStations();
        extern void manageParties();
        extern void manageCandidates();
        extern void manageVoters();
        extern void manageVoting();
        extern void manageResults();
        extern int ShowMenu(ScreenInteractive & screen, 
             const string& heading, 
             const vector<string>& options);

bool loadAdmins(vector<Admin>& adminList) {
    ifstream file(ADMIN_FILE);
    if (!file.is_open()) return false;

    json j;
    file >> j;
    file.close();

    for (const auto& item : j) {
        adminList.push_back(Admin::fromJSON(item));
    }
    return true;
}

bool saveAdmins(const vector<Admin>& adminList) {
    ofstream file(ADMIN_FILE);
    if (!file.is_open()) return false;

    json j;
    for (const auto& admin : adminList) {
        j.push_back(admin.toJSON());
    }
    file << j.dump(4);
    file.close();
    return true;
}

bool loginAdmin(const vector<Admin>& adminList) {
    string inputUser, inputPass;
    auto screen = ScreenInteractive::TerminalOutput();

    //using FTXUI for better input handling
    vector<InputField> fields = {
        {"Username", &inputUser, InputField::TEXT},
        {"Password", &inputPass, InputField::TEXT}
    };
    bool success = ShowForm(screen, "Admin Login", fields);
    if (!success) {
        cout << "\n[ERROR] Login cancelled.\n";
        return false;
    }
    // cout << "\n====================================\n";
    // cout << "         ADMIN LOGIN PORTAL         \n";
    // cout << "====================================\n";
    // cout << "\nUsername: ";
    // getline(cin, inputUser);
    // cout << "Password: ";
    // getline(cin, inputPass);

    // for (const auto& admin : adminList) {
    //     if (admin.username == inputUser && admin.password == inputPass) {
    //         cout << "\n[SUCCESS] Admin authenticated.\n";
    //         return true;
    //     }
    // }

    cout << "\n[ERROR] Invalid credentials. Access denied.\n";
    return false;
}

void adminPanel() {
    system("cls");
    while (true) {

        auto screen = ScreenInteractive::TerminalOutput();

    vector<string> adminPanel = {
        "Elections",
        "Cities",
        "Constituencies",
        "Polling Stations",
        "Political Parties",
        "Candidates",
        "Voters",
        "Voting Process",
        "Results",
        "Exit Admin Panel",
    };

    int choice = ShowMenu(screen, "Admin Panel", adminPanel);
    system("cls");

    switch (choice) {

        case 0: manageElections(); break;
        case 1: manageCities(); break;
        case 2: manageConstituencies(); break;
        case 3: managePollingStations(); break;
        case 4: manageParties(); break;
        case 5: manageCandidates(); break;
        case 6: manageVoters(); break;
        case 7: manageVoting(); break;
        case 8: manageResults(); break;
        case 9: return; // Exit Admin Panel
        default: cout << "Invalid choice. Try again.\n"; break;

    }
}
}