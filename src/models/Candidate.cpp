#include "Candidate.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
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

const string CANDIDATE_FILE = "../../data/candidates.json";

// Helper: Check if candidate ID already exists
bool candidateIDExists(int candidateID, const vector<Candidate> &candidates)
{
    for (const auto &c : candidates)
    {
        if (c.getCandidateID() == candidateID)
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
                    cerr << "⚠️ Invalid candidate data found in file, skipping.\n";
                }
            }
        }
        catch (const std::exception &e)
        {
            cerr << "❌ Error reading candidates file: " << e.what() << endl;
        }
    }
    else
    {
        cerr << "⚠️ Could not open candidate file for reading.\n";
    }
    return candidates;
}

// Save all candidates to file
void saveAllCandidates(const vector<Candidate> &candidates)
{
    ofstream file(CANDIDATE_FILE);
    if (!file.is_open())
    {
        cerr << "❌ Could not open candidate file for writing.\n";
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
            cerr << "⚠️ Skipping invalid candidate during save.\n";
        }
    }
    file << j.dump(4);
}

// Admin: Add a new candidate
void addCandidate(const Candidate &newCandidate)
{
    if (!isValidCandidate(newCandidate))
    {
        cerr << "❌ Invalid candidate data. Please check all fields.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    if (candidateIDExists(newCandidate.getCandidateID(), candidates))
    {
        cerr << "❌ Candidate ID already exists. Use a unique ID.\n";
        return;
    }
    candidates.push_back(newCandidate);
    saveAllCandidates(candidates);
    cout << "✅ Candidate added successfully.\n";
}

// Admin: Edit candidate by ID
void editCandidate(int candidateID, const string &newName, int newPartyID, int newConstituencyID)
{
    if (candidateID <= 0)
    {
        cerr << "❌ Invalid candidate ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    bool found = false;
    for (auto &c : candidates)
    {
        if (c.getCandidateID() == candidateID)
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
        cerr << "❌ Candidate ID not found.\n";
        return;
    }
    saveAllCandidates(candidates);
    cout << "✏️ Candidate updated successfully.\n";
}
// Admin: Delete candidate by ID
void deleteCandidateByID(int candidateID)
{
    if (candidateID <= 0)
    {
        cerr << "❌ Invalid candidate ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    size_t before = candidates.size();
    auto it = remove_if(candidates.begin(), candidates.end(), [candidateID](const Candidate &c)
                        { return c.getCandidateID() == candidateID; });
    candidates.erase(it, candidates.end());
    saveAllCandidates(candidates);
    if (candidates.size() < before)
        cout << "🗑️ Candidate deleted.\n";
    else
        cout << "⚠️ Candidate ID not found.\n";
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
        cerr << "❌ Invalid constituency ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    bool found = false;
    for (const auto &c : candidates)
    {
        if (c.getConstituencyID() == constID)
        {
            cout << "🗳️ " << c.getCandidateID() << " - " << c.getCandidateName() << " (PartyID: " << c.getPartyID() << ")" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No candidates found for this constituency.\n";
    }
}

// User: Get candidate by ID
Candidate *getCandidateByID(int candidateID)
{
    if (candidateID <= 0)
    {
        cerr << "❌ Invalid candidate ID.\n";
        return nullptr;
    }
    vector<Candidate> candidates = loadAllCandidates();
    for (auto &c : candidates)
    {
        if (c.getCandidateID() == candidateID)
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

// int main()
// {
//     // Example usage
//     Candidate c1(1, "John Doe", 101, 201);
//     addCandidate(c1);
//     listAllCandidates();
//     viewCandidatesByConstituency(201);
//     editCandidate(1, "Jane Doe", 102, 202);
//     listAllCandidates();
//     deleteCandidateByID(1);
//     listAllCandidates();
//     return 0;
// }