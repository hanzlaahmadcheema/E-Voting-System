#include "Party.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
// Party
Party::Party() : PartyID(0), PartyName(""), PartySymbol("") {}
Party::Party(int PartyID, const string &PartyName, const string &PartySymbol)
{
    this->PartyID = PartyID;
    this->PartyName = PartyName;
    this->PartySymbol = PartySymbol;
}
void Party::setPartyID(int PartyID)
{
    this->PartyID = PartyID;
}
void Party::setPartyName(const string &PartyName)
{
    this->PartyName = PartyName;
}
void Party::setPartySymbol(const string &PartySymbol)
{
    this->PartySymbol = PartySymbol;
}
int Party::getPartyID() const
{
    return PartyID;
}
string Party::getPartyName() const
{
    return PartyName;
}
string Party::getPartySymbol() const
{
    return PartySymbol;
}
void Party::displayPartyInfo() const
{
    cout << "Party ID: " << PartyID << "\n"
         << "Name: " << PartyName << "\n"
         << "Symbol: " << PartySymbol << endl;
}

json Party::toJSON() const
{
    return json{
        {"PartyID", PartyID},
        {"PartyName", PartyName},
        {"PartySymbol", PartySymbol}};
}

Party Party::fromJSON(const json &j)
{
    return Party(
        j.at("PartyID").get<int>(),
        j.at("PartyName").get<std::string>(),
        j.at("PartySymbol").get<std::string>());
}

const string PARTY_FILE = "../../data/parties.json";

// Helper: Check if PartyID exists
bool partyIDExists(int id, const vector<Party> &list)
{
    for (const auto &p : list)
    {
        if (p.getPartyID() == id)
            return true;
    }
    return false;
}

// Helper: Validate Party Name and Symbol
bool isValidPartyName(const string &name)
{
    return !name.empty() && name.length() <= 50;
}
bool isValidPartySymbol(const string &symbol)
{
    return !symbol.empty() && symbol.length() <= 20;
}

// Load all parties
vector<Party> loadAllParties()
{
    vector<Party> list;
    ifstream file(PARTY_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                list.push_back(Party::fromJSON(obj));
            }
        }
        catch (...)
        {
            cerr << "Error: Invalid party data format.\n";
        }
    }
    return list;
}

// Save all parties
void saveAllParties(const vector<Party> &list)
{
    ofstream file(PARTY_FILE);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open party file for writing.\n";
        return;
    }
    json j;
    for (const auto &p : list)
    {
        j.push_back(p.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add party
void addParty(const Party &p)
{
    vector<Party> list = loadAllParties();
    if (partyIDExists(p.getPartyID(), list))
    {
        cout << "Party ID already exists.\n";
        return;
    }
    if (!isValidPartyName(p.getPartyName()))
    {
        cout << "Invalid party name (empty or too long).\n";
        return;
    }
    if (!isValidPartySymbol(p.getPartySymbol()))
    {
        cout << "Invalid party symbol (empty or too long).\n";
        return;
    }
    list.push_back(p);
    saveAllParties(list);
    cout << "Party added.\n";
}

// Admin: Edit party
void editParty(int id, const string &name, const string &symbol)
{
    vector<Party> list = loadAllParties();
    bool found = false;
    if (!isValidPartyName(name))
    {
        cout << "Invalid party name (empty or too long).\n";
        return;
    }
    if (!isValidPartySymbol(symbol))
    {
        cout << "Invalid party symbol (empty or too long).\n";
        return;
    }
    for (auto &p : list)
    {
        if (p.getPartyID() == id)
        {
            p.setPartyName(name);
            p.setPartySymbol(symbol);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Party ID not found.\n";
        return;
    }
    saveAllParties(list);
    cout << "Party updated.\n";
}

// Admin: Delete party
void deleteParty(int id)
{
    vector<Party> list = loadAllParties();
    auto it = remove_if(list.begin(), list.end(), [id](const Party &p)
                        { return p.getPartyID() == id; });
    if (it == list.end())
    {
        cout << "Party ID not found.\n";
        return;
    }
    list.erase(it, list.end());
    saveAllParties(list);
    cout << "Party deleted.\n";
}

// Admin/User: View all parties
void listAllParties()
{
    vector<Party> list = loadAllParties();
    if (list.empty())
    {
        cout << "No parties found.\n";
        return;
    }
    for (const auto &p : list)
    {
        cout << p.getPartyID() << " | " << p.getPartyName() << " | Symbol: " << p.getPartySymbol() << endl;
    }
}

bool partyExists(int id) {
    vector<Party> list = loadAllParties();
    for (const auto& p : list) {
        if (p.getPartyID() == id) return true;
    }
    return false;
}

void manageParties() {
    int choice;
    while (true) {
        cout << "\n Party Management\n";
        cout << "1. Add Party\n";
        cout << "2. View All Parties\n";
        cout << "3. Edit Party\n";
        cout << "4. Delete Party\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name, symbol;
            cout << "Enter Party ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Party Name: ";
            getline(cin, name);
            cout << "Enter Party Symbol: ";
            getline(cin, symbol);
            Party p(id, name, symbol);
            addParty(p);
        } else if (choice == 2) {
            listAllParties();
        } else if (choice == 3) {
            int id;
            string name, symbol;
            cout << "Enter Party ID to edit: ";
            cin >> id;
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, name);
            cout << "Enter new symbol: ";
            getline(cin, symbol);
            editParty(id, name, symbol);
        } else if (choice == 4) {
            int id;
            cout << "Enter Party ID to delete: ";
            cin >> id;
            deleteParty(id);
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
//     Party p1(1, "Party A", "Symbol A");
//     addParty(p1);
//     listAllParties();
//     editParty(1, "Updated Party A", "Updated Symbol A");
//     listAllParties();
//     deleteParty(1);
//     listAllParties();
//     return 0;
// }