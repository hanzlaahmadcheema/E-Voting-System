#include "Candidate.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern int getNextID(const string &key);
// Candidate
Candidate::Candidate() : CandidateID(0), CandidateName(""), PartyID(0), ConstituencyID(0) {}

Candidate::Candidate(int CandidateID, const string &CandidateName, int PartyID, int ConstituencyID)
{
    this->CandidateID = CandidateID;
    this->CandidateName = CandidateName;
    this->PartyID = PartyID;
    this->ConstituencyID = ConstituencyID;
}

void Candidate::setCandidateID(int CandidateID)
{
    this->CandidateID = CandidateID;
}
void Candidate::setCandidateName(const string &CandidateName)
{
    this->CandidateName = CandidateName;
}
void Candidate::setPartyID(int PartyID)
{
    this->PartyID = PartyID;
}
void Candidate::setConstituencyID(int ConstituencyID)
{
    this->ConstituencyID = ConstituencyID;
}
int Candidate::getCandidateID() const
{
    return CandidateID;
}
string Candidate::getCandidateName() const
{
    return CandidateName;
}
int Candidate::getPartyID() const
{
    return PartyID;
}
int Candidate::getConstituencyID() const
{
    return ConstituencyID;
}

void Candidate::displayCandidateInfo() const
{
    cout << "Candidate ID: " << CandidateID << "\n"
         << "Name: " << CandidateName << "\n"
         << "Party ID: " << PartyID << "\n"
         << "Constituency ID: " << ConstituencyID << endl;
}
// Add any other methods or member functions as needed
json Candidate::toJSON() const
{
    return json{
        {"CandidateID", CandidateID},
        {"CandidateName", CandidateName},
        {"PartyID", PartyID},
        {"ConstituencyID", ConstituencyID}};
}
Candidate Candidate::fromJSON(const json &j)
{
    return Candidate(
        j.at("CandidateID").get<int>(),
        j.at("CandidateName").get<std::string>(),
        j.at("PartyID").get<int>(),
        j.at("ConstituencyID").get<int>());
}

const string CANDIDATE_FILE = "data/candidates.json";

// Helper: Check if candidate ID already exists
bool candidateIDExists(int CandidateID, const vector<Candidate> &candidates)
{
    for (const auto &c : candidates)
    {
        if (c.getCandidateID() == CandidateID)
            return true;
    }
    return false;
}

// Helper: Validate candidate fields
bool isValidCandidate(const Candidate &c)
{
    if (c.getCandidateID() <= 0)
        return false;
    if (c.getCandidateName().empty())
        return false;
    if (c.getPartyID() <= 0)
        return false;
    if (c.getConstituencyID() <= 0)
        return false;
    return true;
}

bool isValidCandidateID(int id)
{
    return id > 0;
}

bool isValidCandidateName(const string &name)
{
    return !name.empty() && name.length() <= 100;
}
bool isValidPartyID(int partyID)
{
    return partyID > 0;
}

bool isValidConstituencyID(int constID)
{
    return constID > 0;
}
// Load all candidates from file
vector<Candidate> loadAllCandidates()
{
    vector<Candidate> candidates;
    ifstream file(CANDIDATE_FILE);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            for (auto &obj : j)
            {
                Candidate cand = Candidate::fromJSON(obj);
                if (isValidCandidate(cand))
                {
                    candidates.push_back(cand);
                }
                else
                {
                    cerr << "Invalid candidate data found in file, skipping.\n";
                }
            }
        }
        catch (const std::exception &e)
        {
            cerr << "Error reading candidates file: " << e.what() << endl;
        }
    }
    else
    {
        cerr << "Could not open candidate file for reading.\n";
    }
    return candidates;
}

// Save all candidates to file
void saveAllCandidates(const vector<Candidate> &candidates)
{
    ofstream file(CANDIDATE_FILE);
    if (!file.is_open())
    {
        cerr << "Could not open candidate file for writing.\n";
        return;
    }
    json j;
    for (const auto &c : candidates)
    {
        if (isValidCandidate(c))
        {
            j.push_back(c.toJSON());
        }
        else
        {
            cerr << "Skipping invalid candidate during save.\n";
        }
    }
    file << j.dump(4);
}

// Admin: Add a new candidate
void addCandidate(const Candidate &newCandidate)
{
    if (!isValidCandidate(newCandidate))
    {
        cerr << "Invalid candidate data. Please check all fields.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    if (candidateIDExists(newCandidate.getCandidateID(), candidates))
    {
        cerr << "Candidate ID already exists. Use a unique ID.\n";
        return;
    }
    candidates.push_back(newCandidate);
    saveAllCandidates(candidates);
    cout << "Candidate added successfully.\n";
}

// Admin: Edit candidate by ID
void editCandidate(int CandidateID, const string &newName, int newPartyID, int newConstituencyID)
{
    if (CandidateID <= 0)
    {
        cerr << "Invalid candidate ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    bool found = false;
    for (auto &c : candidates)
    {
        if (c.getCandidateID() == CandidateID)
        {
            c.setCandidateName(newName);
            c.setPartyID(newPartyID);
            c.setConstituencyID(newConstituencyID);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cerr << "Candidate ID not found.\n";
        return;
    }
    saveAllCandidates(candidates);
    cout << "Candidate updated successfully.\n";
}
// Admin: Delete candidate by ID
void deleteCandidateByID(int CandidateID)
{
    if (CandidateID <= 0)
    {
        cerr << "Invalid candidate ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    size_t before = candidates.size();
    auto it = remove_if(candidates.begin(), candidates.end(), [CandidateID](const Candidate &c)
                        { return c.getCandidateID() == CandidateID; });
    candidates.erase(it, candidates.end());
    saveAllCandidates(candidates);
    if (candidates.size() < before)
        cout << "Candidate deleted.\n";
    else
        cout << "Candidate ID not found.\n";
}

// Admin: List all candidates
void listAllCandidates()
{
    vector<Candidate> candidates = loadAllCandidates();
    if (candidates.empty())
    {
        cout << "No candidates found.\n";
        return;
    }
    for (const auto &c : candidates)
    {
        cout << c.getCandidateID() << " | " << c.getCandidateName() << " | PartyID: " << c.getPartyID() << " | ConstID: " << c.getConstituencyID() << endl;
    }
}

// User: View candidates in a constituency
void viewCandidatesByConstituency(int constID)
{
    if (constID <= 0)
    {
        cerr << "Invalid constituency ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    bool found = false;
    for (const auto &c : candidates)
    {
        if (c.getConstituencyID() == constID)
        {
            cout << c.getCandidateID() << " - " << c.getCandidateName() << " (PartyID: " << c.getPartyID() << ")" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No candidates found for this constituency.\n";
    }
}

// User: Get candidate by ID
Candidate *getCandidateByID(int CandidateID)
{
    if (CandidateID <= 0)
    {
        cerr << "Invalid candidate ID.\n";
        return nullptr;
    }
    vector<Candidate> candidates = loadAllCandidates();
    for (auto &c : candidates)
    {
        if (c.getCandidateID() == CandidateID)
        {
            return new Candidate(c);
        }
    }
    return nullptr;
}

bool candidateExists(int id) {
    vector<Candidate> list = loadAllCandidates();
    for (const auto& c : list) {
        if (c.getCandidateID() == id) return true;
    }
    return false;
}

void manageCandidates() {
    int choice;
    while (true) {
        cout << "\n Candidate Management\n";
        cout << "1. Add Candidate\n";
        cout << "2. View All Candidates\n";
        cout << "3. View by Constituency\n";
        cout << "4. Edit Candidate\n";
        cout << "5. Delete Candidate\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int partyID, constID;
            string name;
            cout << "Enter Candidate Name: "; cin.ignore(); getline(cin, name);
            if (!isValidCandidateName(name)) {
                cout << "Invalid Candidate Name.\n";
                continue;
            }
            cout << "Enter Party ID: "; cin >> partyID;
            if (!isValidPartyID(partyID)) {
                cout << "Invalid Party ID.\n";
                continue;
            }
            cout << "Enter Constituency ID: "; cin >> constID;
            if (!isValidConstituencyID(constID)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            Candidate c(getNextID("CandidateID"), name, partyID, constID);
            addCandidate(c);
        } else if (choice == 2) {
            listAllCandidates();
        } else if (choice == 3) {
            int constID;
            cout << "Enter Constituency ID: ";
            cin >> constID;
            if (!isValidConstituencyID(constID)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            cout << "Candidates in Constituency ID " << constID << ":\n";
            viewCandidatesByConstituency(constID);
        } else if (choice == 4) {
            int id;
            string name;
            int partyID, constID;
            cout << "List of Candidates:\n";
            listAllCandidates();
            cout << "Enter Candidate ID to edit: ";
            cin >> id;
            if (!isValidCandidateID(id)) {
                cout << "Invalid Candidate ID.\n";
                continue;
            }
            if (!candidateExists(id)) {
                cout << "Candidate ID not found.\n";
                continue;
            }
            cout << "Enter New Name: "; cin.ignore(); getline(cin, name);
            if (!isValidCandidateName(name)) {
                cout << "Invalid Candidate Name.\n";
                continue;
            }
            cout << "Enter New Party ID: "; cin >> partyID;
            if (!isValidPartyID(partyID)) {
                cout << "Invalid Party ID.\n";
                continue;
            }
            cout << "Enter New Constituency ID: "; cin >> constID;
            if (!isValidConstituencyID(constID)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            cout << "Editing Candidate ID " << id << "...\n";
            editCandidate(id, name, partyID, constID);
        } else if (choice == 5) {
            int id;
            cout << "Enter Candidate ID to delete: ";
            cin >> id;
            if (!isValidCandidateID(id)) {
                cout << "Invalid Candidate ID.\n";
                continue;
            }
            if (!candidateExists(id)) {
                cout << "Candidate ID not found.\n";
                continue;
            }
            cout << "Deleting Candidate ID " << id << "...\n";
            deleteCandidateByID(id);
        } else if (choice == 0) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}

// int main()
// {
//     // Example usage
//     Candidate c1(getNextID("CandidateID"), "John Doe", 101, 201);
//     addCandidate(c1);
//     listAllCandidates();
//     viewCandidatesByConstituency(201);
//     editCandidate(c1.getCandidateID(), "Jane Doe", 102, 202);
//     listAllCandidates();
//     deleteCandidateByID(c1.getCandidateID());
//     listAllCandidates();
//     return 0;
// }