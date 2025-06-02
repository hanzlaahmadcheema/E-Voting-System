#include "AdminCLI.h"
#include <fstream>
#include <iostream>
#include <limits>

using json = nlohmann::json;
using namespace std;
const std::string ADMIN_FILE = "data/admins.json";


void manageElections();
void manageCities();
void manageConstituencies();
void managePollingStations();
void manageParties();
void manageCandidates();
void manageVoters();
void manageVoting();
void manageResults();

bool loadAdmins(std::vector<Admin>& adminList) {
    std::ifstream file(ADMIN_FILE);
    if (!file.is_open()) return false;

    json j;
    file >> j;
    file.close();

    for (const auto& item : j) {
        adminList.push_back(Admin::fromJSON(item));
    }
    return true;
}

bool saveAdmins(const std::vector<Admin>& adminList) {
    std::ofstream file(ADMIN_FILE);
    if (!file.is_open()) return false;

    json j;
    for (const auto& admin : adminList) {
        j.push_back(admin.toJSON());
    }
    file << j.dump(4);
    file.close();
    return true;
}

bool loginAdmin(const std::vector<Admin>& adminList) {
    std::string inputUser, inputPass;

    std::cout << "\n====================================\n";
    std::cout << "         ADMIN LOGIN PORTAL         \n";
    std::cout << "====================================\n";
    std::cout << "\nUsername: ";
    std::getline(std::cin, inputUser);
    std::cout << "Password: ";
    std::getline(std::cin, inputPass);

    for (const auto& admin : adminList) {
        if (admin.username == inputUser && admin.password == inputPass) {
            std::cout << "\n[SUCCESS] Admin authenticated.\n";
            return true;
        }
    }

    std::cout << "\n[ERROR] Invalid credentials. Access denied.\n";
    return false;
}

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
