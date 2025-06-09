#include <custom/config.h>


extern int getNextID(const string &key);
void deleteVotesByElectionID(int ElectionID);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

// Election
Election::Election() : ElectionID(0), ElectionName(""), ElectionType(""), ElectionDate("") {}
Election::Election(int ElectionID, const string &ElectionName, const string &ElectionType, const string &ElectionDate)
{
    this->ElectionID = ElectionID;
    this->ElectionName = ElectionName;
    this->ElectionType = ElectionType;
    this->ElectionDate = ElectionDate;
}
void Election::setElectionID(int ElectionID)
{
    this->ElectionID = ElectionID;
}
void Election::setElectionName(const string &ElectionName)
{
    this->ElectionName = ElectionName;
}
void Election::setElectionType(const string &ElectionType)
{
    this->ElectionType = ElectionType;
}
void Election::setElectionDate(const string &ElectionDate)
{
    this->ElectionDate = ElectionDate;
}
int Election::getElectionID() const
{
    return ElectionID;
}
string Election::getElectionName() const
{
    return ElectionName;
}
string Election::getElectionType() const
{
    return ElectionType;
}
string Election::getElectionDate() const
{
    return ElectionDate;
}
void Election::displayElectionInfo() const
{
    cout << "Election ID: " << ElectionID << "\n"
         << "Name: " << ElectionName << "\n"
         << "Type: " << ElectionType << "\n"
         << "Date: " << ElectionDate << endl;
}

json Election::toJSON() const
{
    return json{
        {"ElectionID", ElectionID},
        {"ElectionName", ElectionName},
        {"ElectionType", ElectionType},
        {"ElectionDate", ElectionDate}};
}
Election Election::fromJSON(const json &j)
{
    return Election(
        j.at("ElectionID").get<int>(),
        j.at("ElectionName").get<string>(),
        j.at("ElectionType").get<string>(),
        j.at("ElectionDate").get<string>());
}

const string ELECTION_FILE = "data/elections.json";

#include <regex>

bool isValidElectionID(int id)
{
    return id > 0;
}
bool isValidElectionName(const string &name)
{
    return !name.empty() && name.length() <= 100;
}
bool isValidElectionType(const string &type)
{
    //should be one of the predefined types
    //NA, PP، PS, PK, PB
    return !type.empty() && type.length() <= 50 &&
           (type == "NA" || type == "PP" || type == "PS" || type == "PK" || type == "PB");
}
bool isValidElectionDate(const string &date)
{
    // YYYY-MM-DD format
    regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
    return regex_match(date, date_regex);
}

vector<Election> loadAllElections()
{
    vector<Election> list;
    ifstream file(ELECTION_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                try
                {
                    Election e = Election::fromJSON(obj);
                    if (isValidElectionID(e.getElectionID()) &&
                        isValidElectionName(e.getElectionName()) &&
                        isValidElectionType(e.getElectionType()) &&
                        isValidElectionDate(e.getElectionDate()))
                    {
                        list.push_back(e);
                    }
                }
                catch (...)
                {
                    // Skip invalid entry
                    continue;
                }
            }
        }
        catch (...)
        {
            // Invalid JSON, return empty list
            cerr << "Error: Invalid JSON format in elections file.\n";
            return list;
        }
    }
    return list;
}

// Save elections
void saveAllElections(const vector<Election> &list)
{
    ofstream file(ELECTION_FILE);
    json j;
    for (const auto &e : list)
    {
        j.push_back(e.toJSON());
    }
    file << j.dump(4);
}

// Admin: Create election
void createElection(const Election &e)
{
    if (!isValidElectionID(e.getElectionID()))
    {
        cout << "Invalid Election ID.\n";
        return;
    }
    if (!isValidElectionName(e.getElectionName()))
    {
        cout << "Invalid Election Name.\n";
        return;
    }
    if (!isValidElectionType(e.getElectionType()))
    {
        cout << "Invalid Election Type.\n";
        return;
    }
    if (!isValidElectionDate(e.getElectionDate()))
    {
        cout << "Invalid Election Date. Use YYYY-MM-DD.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    for (const auto &existing : list)
    {
        if (existing.getElectionID() == e.getElectionID())
        {
            cout << "Election ID already exists.\n";
            return;
        }
    }
    list.push_back(e);
    saveAllElections(list);
    cout << "Election created.\n";
}

// Admin: Edit election
void editElection(int ElectionID, const string &newName, const string &newType, const string &newDate)
{
    if (!isValidElectionID(ElectionID))
    {
        cout << "Invalid Election ID.\n";
        return;
    }
    if (!isValidElectionName(newName))
    {
        cout << "Invalid Election Name.\n";
        return;
    }
    if (!isValidElectionType(newType))
    {
        cout << "Invalid Election Type.\n";
        return;
    }
    if (!isValidElectionDate(newDate))
    {
        cout << "Invalid Election Date. Use YYYY-MM-DD.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    bool found = false;
    for (auto &e : list)
    {
        if (e.getElectionID() == ElectionID)
        {
            e.setElectionName(newName);
            e.setElectionType(newType);
            e.setElectionDate(newDate);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Election ID not found.\n";
        return;
    }
    saveAllElections(list);
    cout << "Election updated.\n";
}

// Admin: Delete election
void deleteElection(int ElectionID)
{
    if (!isValidElectionID(ElectionID))
    {
        cout << "Invalid Election ID.\n";
        return;
    }
    vector<Election> list = loadAllElections();
    auto it = remove_if(list.begin(), list.end(), [ElectionID](const Election &e)
                        { return e.getElectionID() == ElectionID; });
    if (it == list.end())
    {
        cout << "Election ID not found.\n";
        return;
    }
    list.erase(it, list.end());
    deleteVotesByElectionID(ElectionID);
    saveAllElections(list);
    cout << "Election deleted.\n";
}

// Admin/User: List all elections

//get type by id
string getElectionTypeByID(int id)
{
    vector<Election> list = loadAllElections();
    for (const auto &e : list)
    {
        if (e.getElectionID() == id)
        {
            return e.getElectionType();
        }
    }
    return "";
}

void listAllElections() {
    vector<Election> list = loadAllElections();
    if (list.empty()) {
        cout << "No elections found.\n";
        return;
    }

    vector<string> headers = {"ID", "Name", "Type", "Date"};
    vector<vector<string>> data;

    for (const auto& e : list) {
        data.push_back({
            to_string(e.getElectionID()),
            e.getElectionName(),
            e.getElectionType(),
            e.getElectionDate()
        });
    }

    ShowTableFTXUI("All Elections", headers, data);
}


bool electionExists(int id) {
    vector<Election> list = loadAllElections();
    for (const auto& e : list) {
        if (e.getElectionID() == id) return true;
    }
    return false;
}

void manageElections() {
    while (true) {

        auto screen = ScreenInteractive::TerminalOutput();

    vector<string> electionManagement = {
        "Create Election",
        "View All Elections",
        "Edit Election",
        "Delete Election",
        "Back"
    };

    int choice = ShowMenu(screen, "Election Management", electionManagement);

        if (choice == 0) {
            string name, type, date;
        auto screen = ScreenInteractive::TerminalOutput();

            vector<InputField> form = {
                {"Election Name", &name, InputField::TEXT},
                {"Election Type", &type, InputField::DROPDOWN, {"NA", "PP", "PS", "PK", "PB"}},
                {"Election Date", &date, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Create Election", form);
            if (success) {
    // Run your validation
    if (isValidElectionName(name) && isValidElectionType(type) && isValidElectionDate(date)) {
        Election e(getNextID("ElectionID"), name, type, date);
        createElection(e);
    } else {
        cout << "Validation failed.\n";
    }
} else {
    cout << "Form was not submitted.\n";
}
        } else if (choice == 1) {
            listAllElections();
        } else if (choice == 2) {
            string id_str, name, type, date;
            listAllElections();
            auto screen = ScreenInteractive::TerminalOutput();
            vector<InputField> form = {
                {"Election ID", &id_str, InputField::TEXT},
                {"Election Name", &name, InputField::TEXT},
                {"Election Type", &type, InputField::DROPDOWN, {"NA", "PP", "PS", "PK", "PB"}},
                {"Election Date (YYYY-MM-DD)", &date, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Edit Election", form);
            if (!success) {
                cout << "\n[ERROR] Edit cancelled.\n";
                continue;
            }
            int id = stoi(id_str);
            if (!isValidElectionID(id)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            if (!electionExists(id)) {
                cout << "Election ID not found.\n";
                continue;
            }
            if (!isValidElectionName(name)) {
                cout << "Invalid Election Name.\n";
                continue;
            }
            if (!isValidElectionType(type)) {
                cout << "Invalid Election Type.\n";
                continue;
            }
            if (!isValidElectionDate(date)) {
                cout << "Invalid Election Date. Use YYYY-MM-DD.\n";
                continue;
            }
            editElection(id, name, type, date);
        } else if (choice == 3) {
            string id_str;
            listAllElections();
            auto screen = ScreenInteractive::TerminalOutput();
            vector<InputField> form = {
                {"Election ID", &id_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Delete Election", form);
            if (!success) {
                cout << "\n[ERROR] Edit cancelled.\n";
                continue;
            }
            int id = stoi(id_str);
            if (!isValidElectionID(id)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            if (!electionExists(id)) {
                cout << "Election ID not found.\n";
                continue;
            }
            deleteElection(id);
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
//     Election e1(getNextID("ElectionID"), "Presidential Election", "Presidential", "2024-11-05");
//     createElection(e1);
//     listAllElections();
//     editElection(e1.getElectionID(), "Updated Presidential Election", "Presidential", "2024-11-06");
//     listAllElections();
//     deleteElection(e1.getElectionID());
//     listAllElections();
//     return 0;
// }