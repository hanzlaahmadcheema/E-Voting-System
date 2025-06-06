#include "Voter.h"
#include "../core/Universal.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;
using namespace ftxui;

extern int getNextID(const string& key);
extern bool pollingStationExists(int id);
extern bool constituencyExists(int id);
extern void listAllStations();
extern void listAllConstituencies();
extern int getVoterIDByCNIC(const string &VoterCNIC);
extern void deleteVotesByVoterID(int voterID);
extern void listCitiesByProvince(const string &province);
extern void listConstituenciesByCity(int cityID);
extern void listStationsByConstituency(int constChoice);
extern void listStationsByCity(int cityID);
extern int ShowMenu(ScreenInteractive & screen, 
     const std::string& heading, 
     const std::vector<std::string>& options);


// Voter
Voter::Voter() : VoterID(0), VoterName(""), VoterCNIC(""), VoterGender(""), VoterAge(0), VoterAddress(""), PollingStationID(0) {}
Voter::Voter(int VoterID, const string &VoterName, const string &VoterCNIC, const string &VoterGender, int VoterAge, const string &VoterAddress, int PollingStationID)
{
    this->VoterID = VoterID;
    this->VoterName = VoterName;
    this->VoterCNIC = VoterCNIC;
    this->VoterGender = VoterGender;
    this->VoterAge = VoterAge;
    this->VoterAddress = VoterAddress;
    this->PollingStationID = PollingStationID;
}
void Voter::setVoterID(int VoterID)
{
    this->VoterID = VoterID;
}
void Voter::setVoterName(const string &VoterName)
{
    this->VoterName = VoterName;
}
void Voter::setVoterCNIC(const string &VoterCNIC)
{
    this->VoterCNIC = VoterCNIC;
}
void Voter::setVoterGender(const string &VoterGender)
{
    this->VoterGender = VoterGender;
}
void Voter::setVoterAge(int VoterAge)
{
    this->VoterAge = VoterAge;
}
void Voter::setVoterAddress(const string &VoterAddress)
{
    this->VoterAddress = VoterAddress;
}
void Voter::setPollingStationID(int PollingStationID)
{
    this->PollingStationID = PollingStationID;
}
int Voter::getVoterID() const
{
    return VoterID;
}
string Voter::getVoterName() const
{
    return VoterName;
}
string Voter::getVoterCNIC() const
{
    return VoterCNIC;
}
string Voter::getVoterGender() const
{
    return VoterGender;
}
int Voter::getVoterAge() const
{
    return VoterAge;
}
string Voter::getVoterAddress() const
{
    return VoterAddress;
}
int Voter::getPollingStationID() const
{
    return PollingStationID;
}

json Voter::toJSON() const
{
    return json{
        {"VoterID", VoterID},
        {"VoterName", VoterName},
        {"VoterCNIC", VoterCNIC},
        {"VoterGender", VoterGender},
        {"VoterAge", VoterAge},
        {"VoterAddress", VoterAddress},
        {"PollingStationID", PollingStationID}};
}

Voter Voter::fromJSON(const json &j)
{
    return Voter(
        j.at("VoterID").get<int>(),
        j.at("VoterName").get<std::string>(),
        j.at("VoterCNIC").get<std::string>(),
        j.at("VoterGender").get<std::string>(),
        j.at("VoterAge").get<int>(),
        j.at("VoterAddress").get<std::string>(),
        j.at("PollingStationID").get<int>());
}

const string VOTER_FILE = "data/voters.json";

bool isValidCNIC(const string &VoterCNIC)
{
    if (VoterCNIC.length() != 13)
        return false;
    for (char ch : VoterCNIC)
    {
        if (!isdigit(ch))
            return false;
    }
    return true;
}

// Helper: Validate name (non-empty, letters and spaces)
bool isValidName(const string &name)
{
    if (name.empty() && name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos)
        return false;
    for (char ch : name)
    {
        if (!isalpha(ch) && ch != ' ')
            return false;
    }
    return true;
}

// Helper: Validate gender (Male/Female/Other)
bool isValidGender(const string &gender)
{
    string g = gender;
    for (auto &c : g)
        c = tolower(c);
    return g == "male" || g == "female" || g == "other";
}

// Helper: Validate age (18-120)
bool isValidAge(int age)
{
    return age >= 18 && age <= 120;
}

// Helper: Validate address (non-empty)
bool isValidAddress(const string &addr)
{
    return !addr.empty();
}

// Helper: Validate IDs (positive)
bool isValidID(int id)
{
    return id > 0;
}

// Load all voters from JSON file
vector<Voter> loadAllVoters()
{
    vector<Voter> voters;
    ifstream file(VOTER_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                voters.push_back(Voter::fromJSON(obj));
            }
        }
        catch (...)
        {
            cout << "Error: Failed to parse voters file.\n";
        }
    }
    return voters;
}

// Save all voters to JSON file
void saveAllVoters(const vector<Voter> &voters)
{
    ofstream file(VOTER_FILE);
    json j;
    for (const auto &v : voters)
    {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

// Admin: Register new voter (with duplicate CNIC check and validations)
void registerVoter(const Voter &newVoter)
{
    // Field validations
    if (!isValidID(newVoter.getVoterID()))
    {
        cout << "Error: Invalid Voter ID.\n";
        return;
    }
    if (!isValidName(newVoter.getVoterName()))
    {
        cout << "Error: Invalid name. Only letters and spaces allowed.\n";
        return;
    }
    if (!isValidCNIC(newVoter.getVoterCNIC()))
    {
        cout << "Error: Invalid CNIC. Must be 13 digits.\n";
        return;
    }
    if (!isValidGender(newVoter.getVoterGender()))
    {
        cout << "Error: Invalid gender. Use Male, Female, or Other.\n";
        return;
    }
    if (!isValidAge(newVoter.getVoterAge()))
    {
        cout << "Error: Invalid age. Must be between 18 and 120.\n";
        return;
    }
    if (!isValidAddress(newVoter.getVoterAddress()))
    {
        cout << "Error: Address cannot be empty.\n";
        return;
    }
    if (!isValidID(newVoter.getPollingStationID()))
    {
        cout << "Error: Invalid Polling Station ID.\n";
        return;
    }

    vector<Voter> voters = loadAllVoters();
    // Check for duplicate CNIC
    for (const auto &v : voters)
    {
        if (v.getVoterCNIC() == newVoter.getVoterCNIC())
        {
            cout << "Error: Voter with this CNIC already exists.\n";
            return;
        }
        if (v.getVoterID() == newVoter.getVoterID())
        {
            cout << "Error: Voter with this ID already exists.\n";
            return;
        }
    }
    voters.push_back(newVoter);
    saveAllVoters(voters);
    // Verify if added
    vector<Voter> updatedVoters = loadAllVoters();
    bool found = false;
    for (const auto &v : updatedVoters)
    {
        if (v.getVoterCNIC() == newVoter.getVoterCNIC())
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        cout << "Voter registered successfully.\n";
    }
    else
    {
        cout << "Error: Failed to add voter.\n";
    }
}

// Admin: List all voters
void listAllVoters()
{
    vector<Voter> voters = loadAllVoters();
    if (voters.empty())
    {
        cout << "No voters found.\n";
        return;
    }
    for (const auto &v : voters)
    {
        cout << v.getVoterID() << " | " << v.getVoterName() << " | " << v.getVoterCNIC() << " | "
             << v.getVoterGender() << " | " << v.getVoterAge() << " | " << v.getVoterAddress() << " | "
             << v.getPollingStationID() << "\n";
    }
}

// Admin: Edit voter by CNIC (with check)
void editVoterByCNIC(const string &VoterCNIC, const Voter &updatedVoter)
{
    if (!isValidCNIC(VoterCNIC))
    {
        cout << "Error: Invalid CNIC format.\n";
        return;
    }
    vector<Voter> voters = loadAllVoters();
    bool found = false;
    for (auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            v = updatedVoter; // Update the voter
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Error: Voter with CNIC " << VoterCNIC << " not found.\n";
        return;
    }
    saveAllVoters(voters);
    cout << "Voter updated successfully.\n";
}

void deleteVoterByCNIC(const string &VoterCNIC)
{
    if (!isValidCNIC(VoterCNIC))
    {
        cout << "Error: Invalid CNIC format.\n";
        return;
    }
    vector<Voter> voters = loadAllVoters();
    size_t before = voters.size();
    int deletedVoterID = getVoterIDByCNIC(VoterCNIC);

    // Remove the voter from the list
    auto it = remove_if(voters.begin(), voters.end(), [VoterCNIC](const Voter &v)
                        { return v.getVoterCNIC() == VoterCNIC; });
    voters.erase(it, voters.end());

    // Delete votes associated with this voter
    if (deletedVoterID != -1) {
        deleteVotesByVoterID(deletedVoterID);
    }

    saveAllVoters(voters);
    size_t after = voters.size();

    if (after < before)
    {
        cout << "Voter deleted successfully.\n";
    }
    else
    {
        cout << "Error: Voter with CNIC " << VoterCNIC << " not found.\n";
    }
}

// User: Login by CNIC (returns voter object or null, with check)
Voter *loginByCNIC(const string &VoterCNIC)
{
    if (!isValidCNIC(VoterCNIC))
    {
        cout << "Error: Invalid CNIC format.\n";
        return nullptr;
    }
    vector<Voter> voters = loadAllVoters();
    for (auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            return new Voter(v);
        }
    }
    cout << "Error: No voter found with this CNIC.\n";
    return nullptr;
}

//function to get ID from Voter CNIC
int getVoterIDByCNIC(const string &VoterCNIC)
{
    vector<Voter> voters = loadAllVoters();
    for (const auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            return v.getVoterID();
        }
    }
    return -1; // Not found
}

// User: View own profile
void viewProfile(const Voter &v)
{
    cout << "Voter Profile\n";
    cout << "ID: " << v.getVoterID() << "\n";
    cout << "Name: " << v.getVoterName() << "\n";
    cout << "CNIC: " << v.getVoterCNIC() << "\n";
    cout << "Gender: " << v.getVoterGender() << "\n";
    cout << "Age: " << v.getVoterAge() << "\n";
    cout << "Address: " << v.getVoterAddress() << "\n";
    cout << "Polling Station ID: " << v.getPollingStationID() << "\n";
}


bool voterExists(string VoterCNIC) {
    vector<Voter> list = loadAllVoters();
    for (const auto& v : list) {
        if (v.getVoterCNIC() == VoterCNIC) return true;
    }
    return false;
}

void manageVoters() {
    int provChoice, cityChoice;
    while (true) {
        auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> voterManagement = {
        "Register Voter",
        "View All Voters",
        "Edit Voter",
        "Delete Voter",
        "Back"
    };

    int choice = ShowMenu(screen, "Voter Management", voterManagement);

        if (choice == 0) {
            int age, pollingID;
            string name, VoterCNIC, gender, address;
            cin.ignore();
            cout << "Name: "; getline(cin, name);
            if (!isValidName(name)) {
                cout << "Invalid name. Only letters and spaces allowed.\n";
                continue;
            }
            cout << "CNIC: "; getline(cin, VoterCNIC);
            if (!isValidCNIC(VoterCNIC)) {
                cout << "Invalid CNIC format.\n";
                continue;
            }
            cout << "Age: "; cin >> age;
            if (!isValidAge(age)) {
                cout << "Invalid age. Age must be positive or greater than 18.\n";
                continue;
            }
            cout << "Gender: "; cin >> gender;
            if (!isValidGender(gender)) {
                cout << "Invalid gender. Please enter Male or Female.\n";
                continue;
            }
            cin.ignore();
                        cout << "Select Province: " << endl;
            cout << "1. Punjab\n" 
                    "2. KPK\n" 
                    "3. Sindh\n"
                    "4. Balochistan" << endl;
            cin >> provChoice;
            if (provChoice < 1 || provChoice > 4) {
                cout << "Invalid province choice.\n";
                continue;
            }
            cin.ignore();
            if (provChoice == 1) {
                listCitiesByProvince("Punjab");
            } else if (provChoice == 2) {
                listCitiesByProvince("KPK");
            } else if (provChoice == 3) {
                listCitiesByProvince("Sindh");
            } else if (provChoice == 4) {
                listCitiesByProvince("Balochistan");
            }
            cout << "Select City:";
            cin >> cityChoice;
            cin.ignore();
            cout << "Address: "; getline(cin, address);
            if (!isValidAddress(address)) {
                cout << "Invalid address. Only letters, numbers, and spaces allowed.\n";
                continue;
            }
            listStationsByCity(cityChoice);
            cout << "Polling Station ID: "; cin >> pollingID;
            if (!pollingStationExists(pollingID)) {
                cout << "Invalid Polling Station ID.\n";
                continue;
            }
            Voter v(getNextID("VoterID"), name, VoterCNIC, gender, age, address, pollingID);
            registerVoter(v);
        } else if (choice == 1) {
            listAllVoters();
        } else if (choice == 2) {
            int age, pollingID;
            string name, VoterCNIC, gender, address;
            cout << "Enter Voter CNIC to edit: ";
            cin >> VoterCNIC;
            if (!voterExists(VoterCNIC)) {
                cout << "Voter with this CNIC does not exist.\n";
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                cout << "Invalid CNIC format.\n";
                continue;
            }
            cin.ignore();
            cout << "New Name: "; getline(cin, name);
            if (!isValidName(name)) {
                cout << "Invalid name. Only letters and spaces allowed.\n";
                continue;
            }
            cout << "New CNIC: "; getline(cin, VoterCNIC);
            if (!isValidCNIC(VoterCNIC)) {
                cout << "Invalid CNIC format.\n";
                continue;
            }
            cout << "New Age: "; cin >> age;
            if (!isValidAge(age)) {
                cout << "Invalid age. Age must be positive or greater than 18.\n";
                continue;
            }
            cin.ignore();
            cout << "New Gender: "; getline(cin, gender);
            if (!isValidGender(gender)) {
                cout << "Invalid gender. Please enter Male or Female.\n";
                continue;
            }
            cout << "New Address: "; getline(cin, address);
            if (!isValidAddress(address)) {
                cout << "Invalid address. Only letters, numbers, and spaces allowed.\n";
                continue;
            }
            listAllStations();
            cout << "New Polling Station ID: "; cin >> pollingID;
            if (!pollingStationExists(pollingID)) {
                cout << "Invalid Polling Station ID.\n";
                continue;
            }
            editVoterByCNIC(VoterCNIC, Voter(getVoterIDByCNIC(VoterCNIC), name, VoterCNIC, gender, age, address, pollingID));
        } else if (choice == 3) {
            string VoterCNIC;
            cout << "Enter Voter CNIC to delete: ";
            cin >> VoterCNIC;
            if (!voterExists(VoterCNIC)) {
                cout << "Voter with this CNIC does not exist.\n";
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                cout << "Invalid CNIC format.\n";
                continue;
            }
            deleteVoterByCNIC(VoterCNIC);
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}


// int main()
// {
//     // Example usage
//     Voter v1(getNextID("VoterID"), "John Doe", "1234567890123", "Male", 30, "123 Main St", 101, 1);
//     registerVoter(v1);
//     listAllVoters();
//     editVoterByCNIC(v1.getVoterID(), Voter(v1.getVoterID(), "John Doe", "1234567890123", "Male", 31, "123 Main St", 101, 1));
//     listAllVoters();
//     deleteVoterByCNIC(v1.getVoterID());
//     listAllVoters();
//     Voter *loggedInVoter = loginByCNIC("1234567890123");
//     if (loggedInVoter)
//     {
//         viewProfile(*loggedInVoter);
//         delete loggedInVoter; // Clean up
//     }
//     return 0;
// }