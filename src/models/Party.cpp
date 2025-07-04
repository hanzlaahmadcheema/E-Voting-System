#include <custom/config.h>

extern string toLower(const string& str);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

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

json Party::toJSON() const
{
    return json{
        {"PartyID", PartyID},
        {"PartyName", PartyName},
        {"PartySymbol", PartySymbol}};
}

Party Party::fromJSON(const json &j)
{
    try {
        return Party(
            j.at("PartyID").get<int>(),
            j.at("PartyName").get<string>(),
            j.at("PartySymbol").get<string>());
    } catch (const std::exception& e) {
        cerr << "Error: Invalid party JSON: " << e.what() << endl;
        return Party();
    }
}

const string PARTY_FILE = "D://E-Voting-System/data/parties.json";

bool isValidInt(const string& str) {
    try {
        size_t idx;
        int val = stoi(str, &idx);
        return idx == str.size() && val > 0;
    } catch (...) {
        return false;
    }
}

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
                Party p = Party::fromJSON(obj);
                if (isValidPartyID(p.getPartyID()) && isValidPartyName(p.getPartyName()) && isValidPartySymbol(p.getPartySymbol()))
                    list.push_back(p);
            }
        }
        catch (const std::exception& e)
        {
            cerr << "Error: Invalid party data format: " << e.what() << endl;
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
        cerr << "Error: Cannot open party file for writing." << endl;
        return;
    }
    json j;
    for (const auto &p : list)
    {
        j.push_back(p.toJSON());
    }
    try {
        file << j.dump(4);
    } catch (const std::exception& e) {
        cerr << "Error: Failed to write party file: " << e.what() << endl;
    }
}

bool partyNameExists(const vector<Party> &list, const string &name)
{
    std::unordered_map<std::string, bool> nameMap;
    for (const auto &p : list)
    {
        nameMap[toLower(p.getPartyName())] = true;
    }
    return nameMap.find(toLower(name)) != nameMap.end();
}

bool partySymbolExists(const vector<Party> &list, const string &symbol)
{
    std::unordered_map<std::string, bool> symbolMap;
    for (const auto &p : list)
    {
        symbolMap[toLower(p.getPartySymbol())] = true;
    }
    return symbolMap.find(toLower(symbol)) != symbolMap.end();
}

bool partyExists(int id) {
    std::unordered_map<int, bool> idMap;
    vector<Party> list = loadAllParties();
    for (const auto& p : list) {
        idMap[p.getPartyID()] = true;
    }
    return idMap.find(id) != idMap.end();
}

// Admin: Add party
void addParty(const Party &p)
{
    vector<Party> list = loadAllParties();
    if (partyExists(p.getPartyID()))
    {
        ShowMessage(screen,"Party ID already exists.","error");
        return;
    }
    if (!isValidPartyName(p.getPartyName()))
    {
        ShowMessage(screen,"Invalid party name (empty or too long).","error");
        return;
    }
    if (!isValidPartySymbol(p.getPartySymbol()))
    {
        ShowMessage(screen,"Invalid party symbol (empty or too long).","error");
        return;
    }
    if (partyNameExists(list, p.getPartyName()))
    {
        ShowMessage(screen,"Party Name already exists.","error");
        return;
    }
    if (partySymbolExists(list, p.getPartySymbol()))
    {
        ShowMessage(screen,"Party Symbol already exists.","error");
        return;
    }
    list.push_back(p);
                               ShowProgressBar(screen, "Adding Party...");
    saveAllParties(list);
    ShowMessage(screen, "Party added.","success");
}

// Admin: Edit party
void editParty(int id, const string &name, const string &symbol)
{
    vector<Party> list = loadAllParties();
    bool found = false;
    if (!isValidPartyName(name))
    {
        ShowMessage(screen,"Invalid party name (empty or too long).","error");
        return;
    }
    if (!isValidPartySymbol(symbol))
    {
        ShowMessage(screen,"Invalid party symbol (empty or too long).","error");
        return;
    }
    for (auto &p : list)
    {
        if (p.getPartyID() == id)
        {
            // Check for duplicate name/symbol (excluding self)
            for (const auto& other : list) {
                if (other.getPartyID() != id) {
                    if (toLower(other.getPartyName()) == toLower(name)) {
                        ShowMessage(screen,"Party Name already exists.","error");
                        return;
                    }
                    if (toLower(other.getPartySymbol()) == toLower(symbol)) {
                        ShowMessage(screen,"Party Symbol already exists.","error");
                        return;
                    }
                }
            }
            p.setPartyName(name);
            p.setPartySymbol(symbol);
            found = true;
            break;
        }
    }
    if (!found)
    {
        ShowMessage(screen,"Party ID not found.","error");
        return;
    }
                                ShowProgressBar(screen, "Editing Party...");

    saveAllParties(list);
    ShowMessage(screen,"Party updated.","success");
}

// Admin: Delete party
void deleteParty(int id)
{
    vector<Party> list = loadAllParties();
    auto it = remove_if(list.begin(), list.end(), [id](const Party &p)
                        { return p.getPartyID() == id; });
    if (it == list.end())
    {
        ShowMessage(screen,"Party ID not found.","error");
        return;
    }
    list.erase(it, list.end());
                ShowProgressBar(screen, "Deleting Party...");
    saveAllParties(list);
    ShowMessage(screen,"Party deleted.","success");
}

// Admin/User: View all parties
void listAllParties()
{
    vector<Party> list = loadAllParties();
    if (list.empty())
    {
        ShowMessage(screen,"No parties found.","info");
        return;
    }
    
    vector<string> headers = {"Party ID", "Name", "Symbol"};
    vector<vector<string>> data;
    for (const auto &p : list)
    {
        data.push_back({
            to_string(p.getPartyID()),
            p.getPartyName(),
            p.getPartySymbol()});
    }
    ShowTableFTXUI(screen, "All Parties", headers, data);
}

void manageParties() {
           ShowSpinner(screen, "Loading Parties...");
    while (true) {
        vector<string> partyMenu = {
            "Add Party",
            "View All Parties",
            "Edit Party",
            "Delete Party",
            "Back"
        };

        int choice = ShowMenu(screen, "Party Menu", partyMenu);
        if (choice == 0) {
            string name, symbol;
            vector<InputField> form = {
                {"Party Name", &name, InputField::TEXT},
                {"Party Symbol", &symbol, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Add Party", form);
            if (!success) {
                ShowMessage(screen,"Party creation cancelled.","error");
                continue;
            }
            if (!isValidPartyName(name)) {
                ShowMessage(screen,"Invalid Party Name.","error");
                continue;
            }
            if (partyNameExists(loadAllParties(), name)) {
                ShowMessage(screen,"Party Name already exists.","error");
                continue;
            }
            if (!isValidPartySymbol(symbol)) {
                ShowMessage(screen,"Invalid Party Symbol.","error");
                continue;
            }
            if (partySymbolExists(loadAllParties(), symbol)) {
                ShowMessage(screen,"Party Symbol already exists.","error");
                continue;
            }
            Party p(getNextID("PartyID"), name, symbol);
            addParty(p);

        } else if (choice == 1) {
            listAllParties();
        } else if (choice == 2) {
            string id_str, name, symbol;
            listAllParties();

            vector<InputField> form = {
                {"Party ID", &id_str, InputField::TEXT},
                {"Party Name", &name, InputField::TEXT},
                {"Party Symbol", &symbol, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Edit Party", form);
            if (!success) {
                ShowMessage(screen,"Party Editing cancelled.","error");
                continue;
            }
            if (!isValidInt(id_str)) {
                ShowMessage(screen,"Invalid Party ID.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!partyExists(id)) {
                ShowMessage(screen,"Party ID not found.","error");
                continue;
            }
            if (!isValidPartyName(name)) {
                ShowMessage(screen,"Invalid Party Name.","error");
                continue;
            }
            if (!isValidPartySymbol(symbol)) {
                ShowMessage(screen,"Invalid Party Symbol.","error");
                continue;
            }
            editParty(id, name, symbol);
        } else if (choice == 3) {
            string id_str;
            listAllParties();

            vector<InputField> form = {
                {"Party ID", &id_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Delete Party", form);
            if (!success) {
                ShowMessage(screen,"Party Deletion cancelled.","error");
                continue;
            }
            if (!isValidInt(id_str)) {
                ShowMessage(screen,"Invalid Party ID.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!partyExists(id)) {
                ShowMessage(screen,"Party ID not found.","error");
                continue;
            }
            deleteParty(id);
        } else if (choice == 4) {
            break;
        } else {
            ShowMessage(screen,"Invalid option.","error");
        }
    }
}