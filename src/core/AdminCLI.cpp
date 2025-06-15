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

vector<InputField> fields = {
        {"Username", &inputUser, InputField::TEXT},
        {"Password", &inputPass, InputField::TEXT}
    };
    bool success = ShowForm(screen, "Admin Login", fields);
    if (!success) {
        ShowMessage(screen,"Login cancelled.", "error");
        return false;
    }
    for (const auto& admin : adminList) {
        if (admin.username == inputUser && admin.password == inputPass) {
        ShowMessage(screen,"Login successful. Welcome to the Admin Panel!", "success");
            return true;
        }
    }

    ShowMessage(screen,"Invalid credentials. Access denied.", "error");
    return false;
}

void adminPanel() {
    
    
    
    vector<Admin> adminList;
    if (!loadAdmins(adminList)) {
        ShowMessage(screen, "Failed to load admin data.", "error");
        return;
    }
    loginAdmin(adminList);
    if (!loginAdmin(adminList)) {
        ShowMessage(screen, "Login failed. Exiting admin panel.", "error");
        return;
    }

    while (true) {

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
        case 9: return;
        default: ShowMessage(screen, "Invalid choice. Try again.", "error"); break;

    }
}
}