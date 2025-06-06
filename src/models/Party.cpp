#include "Party.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>

using namespace std;
using namespace ftxui;

extern int getNextID(const string &key);
extern string toLower(const string& str);
extern int ShowMenu(ScreenInteractive & screen, 
     const std::string& heading, 
     const std::vector<std::string>& options);

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

const string PARTY_FILE = "data/parties.json";

bool isValidPartyID(int id)
{
    return id > 0;
}

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

bool partyNameExists(const vector<Party> &list, const string &name)
{
    for (const auto &p : list)
    {
        if (toLower(p.getPartyName()) == toLower(name))
            return true;
    }
    return false;
}

bool partySymbolExists(const vector<Party> &list, const string &symbol)
{
    for (const auto &p : list)
    {
        if (toLower(p.getPartySymbol()) == toLower(symbol))
            return true;
    }
    return false;
}

bool partyExists(int id) {
    vector<Party> list = loadAllParties();
    for (const auto& p : list) {
        if (p.getPartyID() == id) return true;
    }
    return false;
}

// Admin: Add party
void addParty(const Party &p)
{
    vector<Party> list = loadAllParties();
    if (partyExists(p.getPartyID()))
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

void manageParties() {
    int choice;
    while (true) {
           auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> partyMenu = {
        "Add Party",
        "View All Parties",
        "Edit Party",
        "Delete Party",
        "Back"
    };

    int choice = ShowMenu(screen, "Party Menu", partyMenu);
        if (choice == 0) {
            string name, symbol;
            cin.ignore();
            cout << "Enter Party Name: ";
            getline(cin, name);
            if (!isValidPartyName(name)) {
                cout << "Invalid Party Name.\n";
                continue;
            }
            if (partyNameExists(loadAllParties(), name)) {
                cout << "Party Name already exists.\n";
                continue;
            }
            cout << "Enter Party Symbol: ";
            getline(cin, symbol);
            if (!isValidPartySymbol(symbol)) {
                cout << "Invalid Party Symbol.\n";
                continue;
            }
            if (partySymbolExists(loadAllParties(), symbol)) {
                cout << "Party Symbol already exists.\n";
                continue;
            }
            Party p(getNextID("PartyID"), name, symbol);
            addParty(p);
        } else if (choice == 1) {
            listAllParties();
        } else if (choice == 2) {
            int id;
            string name, symbol;
            cout << "List of Parties:\n";
            listAllParties();
            cout << "Enter Party ID to edit: ";
            cin >> id;
            if (!isValidPartyID(id)) {
                cout << "Invalid Party ID.\n";
                continue;
            }
            if (!partyExists(id)) {
                cout << "Party ID not found.\n";
                continue;
            }
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, name);
            if (!isValidPartyName(name)) {
                cout << "Invalid Party Name.\n";
                continue;
            }
            if (partyNameExists(loadAllParties(), name)) {
                cout << "Party Name already exists.\n";
                continue;
            }
            cout << "Enter new symbol: ";
            getline(cin, symbol);
            if (!isValidPartySymbol(symbol)) {
                cout << "Invalid Party Symbol.\n";
                continue;
            }
            if (partySymbolExists(loadAllParties(), symbol)) {
                cout << "Party Symbol already exists.\n";
                continue;
            }
            editParty(id, name, symbol);
        } else if (choice == 3) {
            int id;
            listAllParties();
            cout << "Enter Party ID to delete: ";
            cin >> id;
            if (!isValidPartyID(id)) {
                cout << "Invalid Party ID.\n";
                continue;
            }
            if (!partyExists(id)) {
                cout << "Party ID not found.\n";
                continue;
            }
            deleteParty(id);
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
//     Party p1(getNextID("PartyID"), "Party A", "Symbol A");
//     addParty(p1);
//     listAllParties();
//     editParty(p1.getPartyID(), "Updated Party A", "Updated Symbol A");
//     listAllParties();
//     deleteParty(p1.getPartyID());
//     listAllParties();
//     return 0;
// }