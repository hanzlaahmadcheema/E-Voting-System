#include <custom/config.h>

extern int getNextID(const string &key);
extern bool cityExists(int id);
extern bool electionExists(int id);
extern void listAllCities();
extern void listAllElections();
extern void listCitiesByProvince(const string &province);
extern string getElectionTypeByID(int id);
extern string toLower(const string& str);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

// Constituency
Constituency::Constituency() : ConstituencyID(0), ConstituencyName(""), CityID(0), ElectionID(0) {}
Constituency::Constituency(int ConstituencyID, const string &ConstituencyName, int CityID, int ElectionID)
{
    this->ConstituencyID = ConstituencyID;
    this->ConstituencyName = ConstituencyName;
    this->CityID = CityID;
    this->ElectionID = ElectionID;
}
void Constituency::setConstituencyID(int ConstituencyID)
{
    this->ConstituencyID = ConstituencyID;
}
void Constituency::setConstituencyName(const string &ConstituencyName)
{
    this->ConstituencyName = ConstituencyName;
}
void Constituency::setCityID(int CityID)
{
    this->CityID = CityID;
}
void Constituency::setElectionID(int ElectionID)
{
    this->ElectionID = ElectionID;
}
int Constituency::getConstituencyID() const
{
    return ConstituencyID;
}
string Constituency::getConstituencyName() const
{
    return ConstituencyName;
}
int Constituency::getCityID() const
{
    return CityID;
}
int Constituency::getElectionID() const
{
    return ElectionID;
}
void Constituency::displayConstituencyInfo() const
{
    cout << "Constituency ID: " << ConstituencyID << "\n"
         << "Name: " << ConstituencyName << "\n"
         << "City ID: " << CityID << "\n"
         << "Election ID: " << ElectionID << endl;
}
// Add any other methods or member functions as needed
json Constituency::toJSON() const
{
    return json{
        {"ConstituencyID", ConstituencyID},
        {"ConstituencyName", ConstituencyName},
        {"CityID", CityID},
        {"ElectionID", ElectionID}};
}
Constituency Constituency::fromJSON(const json &j)
{
    return Constituency(
        j.at("ConstituencyID").get<int>(),
        j.at("ConstituencyName").get<string>(),
        j.at("CityID").get<int>(),
        j.at("ElectionID").get<int>());
}

const string CONSTITUENCY_FILE = "data/constituencies.json";

bool constituencyNameExists(const vector<Constituency> &list, const string &name)
{
    for (const auto &p : list)
    {
        if (toLower(p.getConstituencyName()) == toLower(name))
            return true;
    }
    return false;
}

bool isValidConstituencyID(int id)
{
    return id > 0;
}

bool isValidConstituencyName(const string &name)
{
    return !name.empty() && name.length() <= 50;
}

// Load all constituencies
vector<Constituency> loadAllConstituencies()
{
    vector<Constituency> list;
    ifstream file(CONSTITUENCY_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                list.push_back(Constituency::fromJSON(obj));
            }
        }
        catch (...)
        {
            cerr << "Error: Invalid JSON format in constituency file.\n";
        }
    }
    return list;
}

// Save all constituencies
void saveAllConstituencies(const vector<Constituency> &list)
{
    ofstream file(CONSTITUENCY_FILE);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }
    json j;
    for (const auto &c : list)
    {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add constituency
void addConstituency(const Constituency &newConst)
{
    vector<Constituency> list = loadAllConstituencies();

    // Validation: ID positive, unique; Name non-empty, unique; CityID positive
    if (newConst.getConstituencyID() <= 0)
    {
        cout << "Error: Constituency ID must be positive.\n";
        return;
    }
    if (newConst.getConstituencyName().empty())
    {
        cout << "Error: Constituency name cannot be empty.\n";
        return;
    }
    if (constituencyNameExists(list, newConst.getConstituencyName()))
    {
        cout << "Error: Constituency name already exists.\n";
        return;
    }
    if (newConst.getCityID() <= 0)
    {
        cout << "Error: City ID must be positive.\n";
        return;
    }

    list.push_back(newConst);
    saveAllConstituencies(list);
    cout << "Constituency added.\n";
}

// Admin: Edit constituency name
void editConstituency(int id, const string &newName, int CityID, int ElectionID)
{
    vector<Constituency> list = loadAllConstituencies();
    bool found = false;

    if (id <= 0)
    {
        cout << "Error: Invalid constituency ID.\n";
        return;
    }
    if (newName.empty())
    {
        cout << "Error: New name cannot be empty.\n";
        return;
    }
    if (constituencyNameExists(list, newName))
    {
        cout << "Error: Constituency name already exists.\n";
        return;
    }

    for (auto &c : list)
    {
        if (c.getConstituencyID() == id)
        {
            c.setConstituencyName(newName);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Error: Constituency ID not found.\n";
        return;
    }
    saveAllConstituencies(list);
    cout << "Constituency updated.\n";
}

// Admin: Delete constituency by ID
void deleteConstituency(int id)
{
    if (id <= 0)
    {
        cout << "Error: Invalid constituency ID.\n";
        return;
    }
    vector<Constituency> list = loadAllConstituencies();
    size_t before = list.size();
    auto it = remove_if(list.begin(), list.end(), [id](const Constituency &c)
                        { return c.getConstituencyID() == id; });
    list.erase(it, list.end());
    if (list.size() == before)
    {
        cout << "Error: Constituency ID not found.\n";
        return;
    }
    saveAllConstituencies(list);
    cout << "Constituency deleted.\n";
}

// Admin/User: View all constituencies
void listAllConstituencies()
{
    vector<Constituency> list = loadAllConstituencies();
    if (list.empty())
    {
        cout << "No constituencies found.\n";
        return;
    }
    auto screen = ScreenInteractive::TerminalOutput();
    vector<string> headers = {"Constituency ID", "Name", "City ID", "Election ID"};
    vector<vector<string>> data;
    for (const auto &c : list)
    {
        data.push_back({
            to_string(c.getConstituencyID()),
            c.getConstituencyName(),
            to_string(c.getCityID()),
            to_string(c.getElectionID())
        });
    }
    ShowTableFTXUI("All Constituencies", headers, data);
}

// Admin/User: List by city
void listConstituenciesByCity(int cityID)
{
    if (cityID <= 0)
    {
        cout << "Error: Invalid city ID.\n";
        return;
    }
    vector<Constituency> list = loadAllConstituencies();
    bool found = false;
    vector<string> headers = {"Constituency ID", "Name", "City ID", "Election ID"};
    vector<vector<string>> data;
    for (const auto &c : list)
    {
        if (c.getCityID() == cityID)
        {
            data.push_back({
                to_string(c.getConstituencyID()),
                c.getConstituencyName(),
                to_string(c.getCityID()),
                to_string(c.getElectionID())
            });
            found = true;
        }
    }
    if (!found)
    {
        cout << "No constituencies found for this city.\n";
        return;
    }
    ShowTableFTXUI("Constituencies in City " + to_string(cityID), headers, data);
}

bool constituencyExists(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) return true;
    }
    return false;
}

string getConstituencyTypeByID(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) {
                    //the first 2 digits are type
            string type = c.getConstituencyName().substr(0, 2);
            return type;
        }


    }
    return "";
}

int getElectionIDByConstituencyID(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) {
            return c.getElectionID();
        }
    }
    return -1; // Not found
}

void manageConstituencies() {
    int choice;
    vector<Constituency> list = loadAllConstituencies();
    while (true) {
           auto screen = ScreenInteractive::TerminalOutput();

    vector<string> constituencyMenu = {
        "Add Constituency",
        "View All Constituencies",
        "Edit Constituency",
        "Delete Constituency",
        "Back"
    };

    int choice = ShowMenu(screen, "Constituency Management", constituencyMenu);
        if (choice == 0) {
            string cityID_str, ElectionID_str, name, type, fullName;
            listAllElections();
            auto screen = ScreenInteractive::TerminalOutput();
            vector<InputField> form = {
                {"Election ID", &ElectionID_str, InputField::TEXT}
            };
            int ElectionID = stoi(ElectionID_str);
            type = getElectionTypeByID(ElectionID);
            vector<InputField> form2 = {
                {"Constituency Name (" + type + "- )", &name, InputField::TEXT}
            };
            fullName = type + "-" + name;
            bool success2 = ShowForm(screen, "Add Constituency", form2);
            if (!success2) {
                cout << "\n[ERROR] Constituency creation cancelled.\n";
                continue;
            }
            if (!electionExists(ElectionID)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            if (find(type.begin(), type.end(), type) == type.end()) {
                cout << "Invalid Election Type.\n";
                continue;
            }
            if (!isValidConstituencyName(fullName)) {
                cout << "Invalid Constituency Name.\n";
                continue;
            }
            if (constituencyNameExists(list, fullName)) {
                cout << "Constituency Name already exists.\n";
                continue;
            }
            if (type == "NA") {
                listAllCities();
            } else if (type == "PP") {
                listCitiesByProvince("Punjab");
            } else if (type == "PS") {
                listCitiesByProvince("Sindh");
            } else if (type == "PK") {
                listCitiesByProvince("KPK");
            } else if (type == "PB") {
                listCitiesByProvince("Balochistan");
            }
            vector<InputField> cityForm = {
                {"City ID", &cityID_str, InputField::TEXT}
            };
            bool success5 = ShowForm(screen, "Add Constituency", cityForm);
            if (!success5) {
                cout << "\n[ERROR] City selection cancelled.\n";
                continue;
            }
            int cityID = stoi(cityID_str);
            if (!cityExists(cityID)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            Constituency c(getNextID("ConstituencyID"), fullName, cityID, ElectionID);
            addConstituency(c);
        } else if (choice == 1) {
            listAllConstituencies();
        } else if (choice == 2) {
            string id_str, cityID_str, ElectionID_str, provinceName_str;
            string name, type, fullName;
            listAllConstituencies();
            auto screen = ScreenInteractive::TerminalOutput();
            vector<InputField> form = {
                {"Constituency ID", &id_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Edit Constituency", form);
            if (!success) {
                cout << "\n[ERROR] Edit cancelled.\n";
                continue;
            }
            int id = stoi(id_str);
            if (!isValidConstituencyID(id)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            if (!constituencyExists(id)) {
                cout << "Constituency ID doesn't exists.\n";
                continue;
            }
            listAllElections();
            vector<InputField> form2 = {
                {"Election ID", &ElectionID_str, InputField::TEXT}
            };
            bool success2 = ShowForm(screen, "Edit Constituency", form2);
            if (!success2) {
                cout << "\n[ERROR] Edit cancelled.\n";
                continue;
            }
            int ElectionID = stoi(ElectionID_str);
            if (!electionExists(ElectionID)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            type = getElectionTypeByID(ElectionID);
            vector<InputField> form3 = {
                {"Constituency Name (" + type + "- )", &name, InputField::TEXT}
            };
            bool success3 = ShowForm(screen, "Edit Constituency", form3);
            if (!success3) {
                cout << "\n[ERROR] Edit cancelled.\n";
                continue;
            }
            fullName = type + "-" + name;
            if (!isValidConstituencyName(fullName)) {
                cout << "Invalid Constituency Name.\n";
                continue;
            }
            if (constituencyNameExists(list, fullName)) {
                cout << "Constituency Name already exists.\n";
                continue;
            }
            listAllCities();
            vector<InputField> form4 = {
                {"Select Province", &provinceName_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
            };
            bool success4 = ShowForm(screen, "Edit Constituency", form4);
            if (!success4) {
                cout << "\n[ERROR] City selection cancelled.\n";
                continue;
            }
            listCitiesByProvince(provinceName_str);
            vector<InputField> form5 = {
                {"City ID", &cityID_str, InputField::TEXT}
            };
            bool success5 = ShowForm(screen, "Edit Constituency", form5);
            if (!success5) {
                cout << "\n[ERROR] City selection cancelled.\n";
                continue;
            }
            int cityID = stoi(cityID_str);
            if (!cityExists(cityID)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            editConstituency(id, fullName, cityID, ElectionID);
        } else if (choice == 3) {
            string id_str;
            listAllConstituencies();
            auto screen = ScreenInteractive::TerminalOutput();
            vector<InputField> form = {
                {"Constituency ID", &id_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Delete Constituency", form);
            if (!success) {
                cout << "\n[ERROR] Delete cancelled.\n";
                continue;
            }
            int id = stoi(id_str);
            if (!isValidConstituencyID(id)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            if (!constituencyExists(id)) {
                cout << "Constituency ID not found.\n";
                continue;
            }
            deleteConstituency(id);
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
//     Constituency c1(getNextID("ConstituencyID"), "Downtown", 101);
//     addConstituency(c1);
//     listConstituenciesByCity(101);
//     editConstituency(c1.getConstituencyID(), "Uptown");
//     listConstituenciesByCity(101);
//     deleteConstituency(c1.getConstituencyID());
//     listConstituenciesByCity(101);
//     return 0;
// }