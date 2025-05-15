#include "Voter.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
//Voter
Voter::Voter() : VoterID(0), VoterName(""), VoterCNIC(""),  VoterGender(""), VoterAge(0), VoterAddress(""), PollingStationID(0), ConstituencyID(0) {}
Voter::Voter(int VoterID, const string& VoterName, const string& VoterCNIC, const string& VoterGender, int VoterAge, const string& VoterAddress, int PollingStationID, int ConstituencyID) {
    this->VoterID = VoterID;
    this->VoterName = VoterName;
    this->VoterCNIC = VoterCNIC;
    this->VoterGender = VoterGender;
    this->VoterAge = VoterAge;
    this->VoterAddress = VoterAddress;
    this->PollingStationID = PollingStationID;
    this->ConstituencyID = ConstituencyID;
}
void Voter::setVoterID(int VoterID) {
    this->VoterID = VoterID;
}
void Voter::setVoterName(const string& VoterName) {
    this->VoterName = VoterName;
}
void Voter::setVoterCNIC(const string& VoterCNIC) {
    this->VoterCNIC = VoterCNIC;
}
void Voter::setVoterGender(const string& VoterGender) {
    this->VoterGender = VoterGender;
}
void Voter::setVoterAge(int VoterAge) {
    this->VoterAge = VoterAge;
}
void Voter::setVoterAddress(const string& VoterAddress) {
    this->VoterAddress = VoterAddress;
}
void Voter::setPollingStationID(int PollingStationID) {
    this->PollingStationID = PollingStationID;
}
void Voter::setConstituencyID(int ConstituencyID) {
    this->ConstituencyID = ConstituencyID;
}
int Voter::getVoterID() const {
    return VoterID;
}
string Voter::getVoterName() const {
    return VoterName;
}
string Voter::getVoterCNIC() const {
    return VoterCNIC;
}
string Voter::getVoterGender() const {
    return VoterGender;
}
int Voter::getVoterAge() const {
    return VoterAge;
}
string Voter::getVoterAddress() const {
    return VoterAddress;
}
int Voter::getPollingStationID() const {
    return PollingStationID;
}
int Voter::getConstituencyID() const {
    return ConstituencyID;
}

json Voter::toJSON() const {
    return json{
        {"VoterID", VoterID},
        {"VoterName", VoterName},
        {"VoterCNIC", VoterCNIC},
        {"VoterGender", VoterGender},
        {"VoterAge", VoterAge},
        {"VoterAddress", VoterAddress},
        {"PollingStationID", PollingStationID},
        {"ConstituencyID", ConstituencyID}
    };
}

Voter Voter::fromJSON(const json& j) {
    return Voter(
        j.at("VoterID").get<int>(),
        j.at("VoterName").get<std::string>(),
        j.at("VoterCNIC").get<std::string>(),
        j.at("VoterGender").get<std::string>(),
        j.at("VoterAge").get<int>(),
        j.at("VoterAddress").get<std::string>(),
        j.at("PollingStationID").get<int>(),
        j.at("ConstituencyID").get<int>()
    );
}

const string VOTER_FILE = "../../data/voters.json";

// Helper: Validate CNIC (13 digits, all numbers)
bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char ch : cnic) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

// Helper: Validate name (non-empty, letters and spaces)
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char ch : name) {
        if (!isalpha(ch) && ch != ' ') return false;
    }
    return true;
}

// Helper: Validate gender (Male/Female/Other)
bool isValidGender(const string& gender) {
    string g = gender;
    for (auto& c : g) c = tolower(c);
    return g == "male" || g == "female" || g == "other";
}

// Helper: Validate age (18-120)
bool isValidAge(int age) {
    return age >= 18 && age <= 120;
}

// Helper: Validate address (non-empty)
bool isValidAddress(const string& addr) {
    return !addr.empty();
}

// Helper: Validate IDs (positive)
bool isValidID(int id) {
    return id > 0;
}

// Load all voters from JSON file
vector<Voter> loadAllVoters() {
    vector<Voter> voters;
    ifstream file(VOTER_FILE);
    if (file.is_open()) {
        json j;
        try {
            file >> j;
            for (auto& obj : j) {
                voters.push_back(Voter::fromJSON(obj));
            }
        } catch (...) {
            cout << "❌ Error: Failed to parse voters file.\n";
        }
    }
    return voters;
}

// Save all voters to JSON file
void saveAllVoters(const vector<Voter>& voters) {
    ofstream file(VOTER_FILE);
    json j;
    for (const auto& v : voters) {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

// Admin: Register new voter (with duplicate CNIC check and validations)
void registerVoter(const Voter& newVoter) {
    // Field validations
    if (!isValidID(newVoter.getVoterID())) {
        cout << "❌ Error: Invalid Voter ID.\n";
        return;
    }
    if (!isValidName(newVoter.getVoterName())) {
        cout << "❌ Error: Invalid name. Only letters and spaces allowed.\n";
        return;
    }
    if (!isValidCNIC(newVoter.getVoterCNIC())) {
        cout << "❌ Error: Invalid CNIC. Must be 13 digits.\n";
        return;
    }
    if (!isValidGender(newVoter.getVoterGender())) {
        cout << "❌ Error: Invalid gender. Use Male, Female, or Other.\n";
        return;
    }
    if (!isValidAge(newVoter.getVoterAge())) {
        cout << "❌ Error: Invalid age. Must be between 18 and 120.\n";
        return;
    }
    if (!isValidAddress(newVoter.getVoterAddress())) {
        cout << "❌ Error: Address cannot be empty.\n";
        return;
    }
    if (!isValidID(newVoter.getPollingStationID())) {
        cout << "❌ Error: Invalid Polling Station ID.\n";
        return;
    }
    if (!isValidID(newVoter.getConstituencyID())) {
        cout << "❌ Error: Invalid Constituency ID.\n";
        return;
    }

    vector<Voter> voters = loadAllVoters();
    // Check for duplicate CNIC
    for (const auto& v : voters) {
        if (v.getVoterCNIC() == newVoter.getVoterCNIC()) {
            cout << "❌ Error: Voter with this CNIC already exists.\n";
            return;
        }
        if (v.getVoterID() == newVoter.getVoterID()) {
            cout << "❌ Error: Voter with this ID already exists.\n";
            return;
        }
    }
    voters.push_back(newVoter);
    saveAllVoters(voters);
    // Verify if added
    vector<Voter> updatedVoters = loadAllVoters();
    bool found = false;
    for (const auto& v : updatedVoters) {
        if (v.getVoterCNIC() == newVoter.getVoterCNIC()) {
            found = true;
            break;
        }
    }
    if (found) {
        cout << "✅ Voter registered successfully.\n";
    } else {
        cout << "❌ Error: Failed to add voter.\n";
    }
}

// Admin: List all voters
void listAllVoters() {
    vector<Voter> voters = loadAllVoters();
    if (voters.empty()) {
        cout << "⚠️ No voters found.\n";
        return;
    }
    for (const auto& v : voters) {
        cout << v.getVoterID() << " | " << v.getVoterName() << " | " << v.getVoterCNIC() << " | "
             << v.getVoterGender() << " | " << v.getVoterAge() << " | " << v.getVoterAddress() << " | "
             << v.getPollingStationID() << " | " << v.getConstituencyID() << "\n";
    }
}

// Admin: Edit voter by ID (with check)
void editVoterByID(int voterID, const Voter& updatedVoter) {
    if (!isValidID(voterID)) {
        cout << "❌ Error: Invalid Voter ID.\n";
        return;
    }
    vector<Voter> voters = loadAllVoters();
    bool found = false;
    for (auto& v : voters) {
        if (v.getVoterID() == voterID) {
            v = updatedVoter; // Update the voter
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "❌ Error: Voter with ID " << voterID << " not found.\n";
        return;
    }
    saveAllVoters(voters);
    cout << "✏️ Voter updated successfully.\n";
}

// Admin: Delete voter by ID (with check)
void deleteVoterByID(int voterID) {
    if (!isValidID(voterID)) {
        cout << "❌ Error: Invalid Voter ID.\n";
        return;
    }
    vector<Voter> voters = loadAllVoters();
    size_t before = voters.size();
    auto it = remove_if(voters.begin(), voters.end(), [voterID](const Voter& v) {
        return v.getVoterID() == voterID;
    });
    voters.erase(it, voters.end());
    saveAllVoters(voters);
    size_t after = voters.size();
    if (after < before) {
        cout << "🗑️ Voter deleted successfully.\n";
    } else {
        cout << "❌ Error: Voter with ID " << voterID << " not found.\n";
    }
}

// User: Login by CNIC (returns voter object or null, with check)
Voter* loginByCNIC(const string& cnic) {
    if (!isValidCNIC(cnic)) {
        cout << "❌ Error: Invalid CNIC format.\n";
        return nullptr;
    }
    vector<Voter> voters = loadAllVoters();
    for (auto& v : voters) {
        if (v.getVoterCNIC() == cnic) {
            return new Voter(v);
        }
    }
    cout << "❌ Error: No voter found with this CNIC.\n";
    return nullptr;
}

// User: View own profile
void viewProfile(const Voter& v) {
    cout << "👤 Voter Profile\n";
    cout << "ID: " << v.getVoterID() << "\n";
    cout << "Name: " << v.getVoterName() << "\n";
    cout << "CNIC: " << v.getVoterCNIC() << "\n";
    cout << "Gender: " << v.getVoterGender() << "\n";
    cout << "Age: " << v.getVoterAge() << "\n";
    cout << "Address: " << v.getVoterAddress() << "\n";
    cout << "Constituency ID: " << v.getConstituencyID() << "\n";
    cout << "Polling Station ID: " << v.getPollingStationID() << "\n";
}

// int main() {
//     //Example usage
//     Voter v1(1, "John Doe", "1234567890123", "Male", 30, "123 Main St", 101, 1);
//     registerVoter(v1);
//     listAllVoters();
//     editVoterByID(1, Voter(1, "John Doe", "1234567890123", "Male", 31, "123 Main St", 101, 1));
//     listAllVoters();
//     deleteVoterByID(1)f;
//     listAllVoters();
//     Voter* loggedInVoter = loginByCNIC("1234567890123");
//     if (loggedInVoter) {
//         viewProfile(*loggedInVoter);
//         delete loggedInVoter; // Clean up
//     }
//     return 0;
// }