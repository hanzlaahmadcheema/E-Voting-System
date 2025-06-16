#include <custom/config.h>

// Forward declarations
extern int getNextID(const string &key);
void deleteVotesByElectionID(int ElectionID);
extern int ShowMenu(ScreenInteractive & screen, 
    const string& heading, 
    const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                const vector<string>& headers, 
                const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);



const string ELECTION_FILE = "D://E-Voting-System/data/elections.json";

// Election class implementation
Election::Election() : ElectionID(0), ElectionName(""), ElectionType(""), ElectionDate("") {}
Election::Election(int ElectionID, const string &ElectionName, const string &ElectionType, const string &ElectionDate)
    : ElectionID(ElectionID), ElectionName(ElectionName), ElectionType(ElectionType), ElectionDate(ElectionDate) {}

void Election::setElectionID(int ElectionID) { this->ElectionID = ElectionID; }
void Election::setElectionName(const string &ElectionName) { this->ElectionName = ElectionName; }
void Election::setElectionType(const string &ElectionType) { this->ElectionType = ElectionType; }
void Election::setElectionDate(const string &ElectionDate) { this->ElectionDate = ElectionDate; }
int Election::getElectionID() const { return ElectionID; }
string Election::getElectionName() const { return ElectionName; }
string Election::getElectionType() const { return ElectionType; }
string Election::getElectionDate() const { return ElectionDate; }

json Election::toJSON() const {
    return json{
       {"ElectionID", ElectionID},
       {"ElectionName", ElectionName},
       {"ElectionType", ElectionType},
       {"ElectionDate", ElectionDate}
    };
}
Election Election::fromJSON(const json &j) {
    return Election(
       j.at("ElectionID").get<int>(),
       j.at("ElectionName").get<string>(),
       j.at("ElectionType").get<string>(),
       j.at("ElectionDate").get<string>());
}

// Validation functions
bool isValidElectionID(int id) {
    return id > 0;
}
bool isValidElectionName(const string &name) {
    return !name.empty() && name.length() <= 100;
}
bool isValidElectionType(const string &type) {
    static const vector<string> validTypes = {"NA", "PP", "PS", "PK", "PB"};
    return !type.empty() && type.length() <= 50 &&
         find(validTypes.begin(), validTypes.end(), type) != validTypes.end();
}
bool isValidElectionDate(const string &date) {
    regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
    return regex_match(date, date_regex);
}

// Load all elections from file
vector<Election> loadAllElections() {
    vector<Election> list;
    ifstream file(ELECTION_FILE);
    if (!file.is_open()) {
       cerr << "Error: Could not open elections file." << endl;
       return list;
    }
    json j;
    try {
       file >> j;
       for (const auto &obj : j) {
          try {
             Election e = Election::fromJSON(obj);
             if (isValidElectionID(e.getElectionID()) &&
                isValidElectionName(e.getElectionName()) &&
                isValidElectionType(e.getElectionType()) &&
                isValidElectionDate(e.getElectionDate())) {
                list.push_back(e);
             }
          } catch (const std::exception &ex) {
             cerr << "Warning: Skipping invalid election entry. " << ex.what() << endl;
             continue;
          }
       }
    } catch (const std::exception &ex) {
       cerr << "Error: Invalid JSON format in elections file. " << ex.what() << endl;
    }
    return list;
}

// Save all elections to file
void saveAllElections(const vector<Election> &list) {
    ofstream file(ELECTION_FILE);
    if (!file.is_open()) {
       cerr << "Error: Could not open elections file for writing." << endl;
       return;
    }
    json j;
    for (const auto &e : list) {
       j.push_back(e.toJSON());
    }
    file << j.dump(4);
}

// Admin: Create election
void createElection(const Election &e) {
    if (!isValidElectionID(e.getElectionID())) {
       ShowMessage(screen, "Invalid Election ID.", "error");
       return;
    }
    if (!isValidElectionName(e.getElectionName())) {
       ShowMessage(screen, "Invalid Election Name.", "error");
       return;
    }
    if (!isValidElectionType(e.getElectionType())) {
       ShowMessage(screen, "Invalid Election Type.", "error");
       return;
    }
    if (!isValidElectionDate(e.getElectionDate())) {
       ShowMessage(screen, "Invalid Election Date. Use YYYY-MM-DD.", "error");
       return;
    }
    vector<Election> list = loadAllElections();
    for (const auto &existing : list) {
       if (existing.getElectionID() == e.getElectionID()) {
          ShowMessage(screen, "Election ID already exists.", "error");
          return;
       }
    }
    list.push_back(e);
    saveAllElections(list);
    ShowMessage(screen, "Election created.", "success");
}

// Admin: Edit election
void editElection(int ElectionID, const string &newName, const string &newType, const string &newDate) {
    if (!isValidElectionID(ElectionID)) {
       ShowMessage(screen, "Invalid Election ID.", "error");
       return;
    }
    if (!isValidElectionName(newName)) {
       ShowMessage(screen, "Invalid Election Name.", "error");
       return;
    }
    if (!isValidElectionType(newType)) {
       ShowMessage(screen, "Invalid Election Type.", "error");
       return;
    }
    if (!isValidElectionDate(newDate)) {
       ShowMessage(screen, "Invalid Election Date. Use YYYY-MM-DD.", "error");
       return;
    }
    vector<Election> list = loadAllElections();
    bool found = false;
    for (auto &e : list) {
       if (e.getElectionID() == ElectionID) {
          e.setElectionName(newName);
          e.setElectionType(newType);
          e.setElectionDate(newDate);
          found = true;
          break;
       }
    }
    if (!found) {
       ShowMessage(screen, "Election ID not found.", "error");
       return;
    }
    saveAllElections(list);
    ShowMessage(screen, "Election edited.", "success");
}

// Admin: Delete election
void deleteElection(int ElectionID) {
    if (!isValidElectionID(ElectionID)) {
       ShowMessage(screen, "Invalid Election ID.", "error");
       return;
    }
    vector<Election> list = loadAllElections();
    auto it = remove_if(list.begin(), list.end(), [ElectionID](const Election &e) {
       return e.getElectionID() == ElectionID;
    });
    if (it == list.end()) {
       ShowMessage(screen, "Election ID not found.", "error");
       return;
    }
    list.erase(it, list.end());
    deleteVotesByElectionID(ElectionID);
    saveAllElections(list);
    ShowMessage(screen, "Election deleted.", "success");
}

// Get type by id
string getElectionTypeByID(int id) {
    vector<Election> list = loadAllElections();
    for (const auto &e : list) {
       if (e.getElectionID() == id) {
          return e.getElectionType();
       }
    }
    return "";
}

// List all elections
void listAllElections() {
    vector<Election> list = loadAllElections();
    if (list.empty()) {
       ShowMessage(screen, "No elections found.", "info");
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

// Main election management menu
void manageElections() {
    while (true) {
       ShowSpinner(screen, "Loading Elections");
       ShowProgressBar(screen, "Loading Elections");
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
          vector<InputField> form = {
             {"Election Name", &name, InputField::TEXT},
             {"Election Type", &type, InputField::DROPDOWN, {"NA", "PP", "PS", "PK", "PB"}},
             {"Election Date (YYYY-MM-DD)", &date, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Create Election", form);
          if (success) {
             if (!isValidElectionName(name)) {
                ShowMessage(screen, "Invalid Election Name.", "error");
             } else if (!isValidElectionType(type)) {
                ShowMessage(screen, "Invalid Election Type.", "error");
             } else if (!isValidElectionDate(date)) {
                ShowMessage(screen, "Invalid Election Date. Use YYYY-MM-DD.", "error");
             } else {
                Election e(getNextID("ElectionID"), name, type, date);
                createElection(e);
             }
          } else {
             ShowMessage(screen, "Election creation cancelled.", "info");
          }
       } else if (choice == 1) {
          listAllElections();
       } else if (choice == 2) {
          string id_str, name, type, date;
          listAllElections();
          vector<InputField> form = {
             {"Election ID", &id_str, InputField::TEXT},
             {"Election Name", &name, InputField::TEXT},
             {"Election Type", &type, InputField::DROPDOWN, {"NA", "PP", "PS", "PK", "PB"}},
             {"Election Date (YYYY-MM-DD)", &date, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Edit Election", form);
          if (!success) {
             ShowMessage(screen, "Election editing cancelled.", "info");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (const std::exception &ex) {
             ShowMessage(screen, "Invalid Election ID format.", "error");
             continue;
          }
          if (!isValidElectionID(id)) {
             ShowMessage(screen, "Invalid Election ID.", "error");
             continue;
          }
          if (!electionExists(id)) {
             ShowMessage(screen, "Election ID not found.", "error");
             continue;
          }
          if (!isValidElectionName(name)) {
             ShowMessage(screen, "Invalid Election Name.", "error");
             continue;
          }
          if (!isValidElectionType(type)) {
             ShowMessage(screen, "Invalid Election Type.", "error");
             continue;
          }
          if (!isValidElectionDate(date)) {
             ShowMessage(screen, "Invalid Election Date. Use YYYY-MM-DD.", "error");
             continue;
          }
          editElection(id, name, type, date);
       } else if (choice == 3) {
          string id_str;
          listAllElections();
          vector<InputField> form = {
             {"Election ID", &id_str, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Delete Election", form);
          if (!success) {
             ShowMessage(screen, "Election deletion cancelled.", "info");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (const std::exception &ex) {
             ShowMessage(screen, "Invalid Election ID format.", "error");
             continue;
          }
          if (!isValidElectionID(id)) {
             ShowMessage(screen, "Invalid Election ID.", "error");
             continue;
          }
          if (!electionExists(id)) {
             ShowMessage(screen, "Election ID not found.", "error");
             continue;
          }
          deleteElection(id);
       } else if (choice == 4) {
          break;
       } else {
          ShowMessage(screen, "Invalid choice. Try again.", "error");
       }
    }
}
