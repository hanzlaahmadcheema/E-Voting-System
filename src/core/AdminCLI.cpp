#include <custom/config.h>

const string ADMIN_FILE = "D://E-Voting-System/data/admins.json";
extern void manageElections();
extern void manageCities();
extern void manageConstituencies();
extern void managePollingStations();
extern void manageParties();
extern void manageCandidates();
extern void manageVoters();
extern void manageVoting();
extern void manageResults();
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

bool loadAdmins(vector<Admin>& adminList) {
    try {
       ifstream file(ADMIN_FILE);
       if (!file.is_open()) {
          ShowMessage(screen, "Could not open admin file.", "error");
          return false;
       }

       json j;
       file >> j;
       file.close();

       if (!j.is_array()) {
          ShowMessage(screen, "Admin file format invalid.", "error");
          return false;
       }

       for (const auto& item : j) {
          try {
             adminList.push_back(Admin::fromJSON(item));
          } catch (const std::exception& e) {
             ShowMessage(screen, "Error parsing admin entry: " + string(e.what()), "error");
          }
       }
       return true;
    } catch (const std::exception& e) {
       ShowMessage(screen, "Exception loading admins: " + string(e.what()), "error");
       return false;
    }
}

bool saveAdmins(const vector<Admin>& adminList) {
    try {
       ofstream file(ADMIN_FILE);
       if (!file.is_open()) {
          ShowMessage(screen, "Could not open admin file for writing.", "error");
          return false;
       }

       json j;
       for (const auto& admin : adminList) {
          j.push_back(admin.toJSON());
       }
       file << j.dump(4);
       file.close();
       return true;
    } catch (const std::exception& e) {
       ShowMessage(screen, "Exception saving admins: " + string(e.what()), "error");
       return false;
    }
}

bool loginAdmin(const vector<Admin>& adminList) {
    string inputUser, inputPass;

    vector<InputField> fields = {
       {"Username", &inputUser, InputField::TEXT},
       {"Password", &inputPass, InputField::TEXT}
    };
    bool success = ShowForm(screen, "Admin Login", fields);
    if (!success) {
       ShowMessage(screen, "Login cancelled.", "error");
       return false;
    }

    // Validation: check for empty fields
    if (inputUser.empty() || inputPass.empty()) {
       ShowMessage(screen, "Username and password cannot be empty.", "error");
       return false;
    }

    for (const auto& admin : adminList) {
       if (admin.username == inputUser && admin.password == inputPass) {
          ShowMessage(screen, "Login successful. Welcome to the Admin Panel!", "success");
          return true;
       }
    }

    ShowMessage(screen, "Invalid credentials. Access denied.", "error");
    return false;
}

void adminPanel() {
    vector<Admin> adminList;
    if (!loadAdmins(adminList)) {
       ShowMessage(screen, "Failed to load admin data.", "error");
       return;
    }

    int loginAttempts = 0;
    const int maxAttempts = 3;
    while (loginAttempts < maxAttempts) {
       if (loginAdmin(adminList)) {
          break;
       }
       loginAttempts++;
       if (loginAttempts < maxAttempts) {
          ShowMessage(screen, "Try again. Attempts left: " + std::to_string(maxAttempts - loginAttempts), "warning");
       }
    }
    if (loginAttempts == maxAttempts) {
       ShowMessage(screen, "Maximum login attempts exceeded. Exiting admin panel.", "error");
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

       int choice = -1;
       try {
          choice = ShowMenu(screen, "Admin Panel", adminPanel);
       } catch (const std::exception& e) {
          ShowMessage(screen, "Error displaying menu: " + string(e.what()), "error");
          continue;
       }

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