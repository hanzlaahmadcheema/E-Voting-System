#include <custom/config.h>




extern int getNextID(const string &key);
extern string toLower(const string& str);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

;

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
    return Party(
        j.at("PartyID").get<int>(),
        j.at("PartyName").get<string>(),
        j.at("PartySymbol").get<string>());
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
            cerr << "Error: Invalid party data format.";
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
        cerr << "Error: Cannot open party file for writing.";
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
    list.push_back(p);
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
    ShowTableFTXUI("All Parties", headers, data);
}

void manageParties() {
    int choice;
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
                {"Party ID", &name, InputField::TEXT},
                {"Party Name", &name, InputField::TEXT},
                {"Party Symbol", &symbol, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Edit Party", form);
            if (!success) {
                  ShowMessage(screen,"Party Editing cancelled.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!isValidPartyID(id)) {
                  ShowMessage(screen,"Invalid Party ID.","error");
                continue;
            }
            if (!partyExists(id)) {
                  ShowMessage(screen,"Party ID not found.","error");
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
            int id = stoi(id_str);
            if (!isValidPartyID(id)) {
                  ShowMessage(screen,"Invalid Party ID.","error");
                continue;
            }
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