#include <custom/config.h>

// --- External Declarations ---
extern int getNextID(const string &key);
extern void listAllParties();
extern void listAllConstituencies();
extern bool partyExists(int id);
extern bool constituencyExists(int id);
extern vector<PollingStation> loadAllStations();
extern int getPollingStationID();
extern string getConstituencyTypeByID(int id);
extern void deleteVotesByCandidateID(int CandidateID);
extern void listCitiesByProvince(const string &province);
extern void listConstituenciesByCity(int cityID);
extern bool isValidCandidateName(const string &name);
extern int ShowMenu(ScreenInteractive & screen, 
    const string& heading, 
    const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                const vector<string>& headers, 
                const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

;

// --- Candidate Implementation ---
Candidate::Candidate() : CandidateID(0), CandidateName(""), PartyID(0), ConstituencyID(0), ConstituencyType("") {}

Candidate::Candidate(int CandidateID, const string &CandidateName, int PartyID, int ConstituencyID, const string &ConstituencyType)
{
    this->CandidateID = CandidateID;
    this->CandidateName = CandidateName;
    this->PartyID = PartyID;
    this->ConstituencyID = ConstituencyID;
    this->ConstituencyType = ConstituencyType;
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
void Candidate::setConstituencyType(const string &ConstituencyType)
{
    this->ConstituencyType = ConstituencyType;
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
string Candidate::getConstituencyType() const
{
    return ConstituencyType;
}

// --- JSON Serialization ---
json Candidate::toJSON() const
{
    return json{
       {"CandidateID", CandidateID},
       {"CandidateName", CandidateName},
       {"PartyID", PartyID},
       {"ConstituencyID", ConstituencyID},
       {"ConstituencyType", ConstituencyType}};
}
Candidate Candidate::fromJSON(const json &j)
{
    return Candidate(
       j.at("CandidateID").get<int>(),
       j.at("CandidateName").get<string>(),
       j.at("PartyID").get<int>(),
       j.at("ConstituencyID").get<int>(),
       j.at("ConstituencyType").get<string>());
}

const string CANDIDATE_FILE = "data/candidates.json";

// --- Validation Helpers ---
bool isValidCandidate(const Candidate &c)
{
    if (c.getCandidateID() <= 0)
       return false;
    if (!isValidCandidateName(c.getCandidateName()))
       return false;
    if (c.getPartyID() <= 0 || !partyExists(c.getPartyID()))
       return false;
    if (c.getConstituencyID() <= 0 || !constituencyExists(c.getConstituencyID()))
       return false;
    return true;
}

bool isValidCandidateID(int id)
{
    return id > 0;
}

bool isValidCandidateName(const string &name)
{
    if (name.empty() || name.length() > 100)
       return false;
    for (char ch : name) {
       if (!isalpha(ch) && ch != ' ')
          return false;
    }
    return true;
}

// --- File I/O ---
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
             try {
                Candidate cand = Candidate::fromJSON(obj);
                if (isValidCandidate(cand))
                    candidates.push_back(cand);
                else
                    cerr << "Invalid candidate data found in file, skipping." << endl;
             } catch (const exception &e) {
                cerr << "Error parsing candidate: " << e.what() << endl;
             }
          }
       }
       catch (const exception &e)
       {
          cerr << "Error reading candidates file: " << e.what() << endl;
       }
    }
    else
    {
       cerr << "Could not open candidate file for reading." << endl;
    }
    return candidates;
}

void saveAllCandidates(const vector<Candidate> &candidates)
{
    ofstream file(CANDIDATE_FILE);
    if (!file.is_open())
    {
       cerr << "Could not open candidate file for writing." << endl;
       return;
    }
    json j;
    for (const auto &c : candidates)
    {
       if (isValidCandidate(c))
          j.push_back(c.toJSON());
       else
          cerr << "Skipping invalid candidate during save." << endl;
    }
    try {
       file << j.dump(4);
    } catch (const exception &e) {
       cerr << "Error writing candidates file: " << e.what() << endl;
    }
}

// --- Candidate Management ---
void addCandidate(const Candidate &newCandidate)
{
    if (!isValidCandidate(newCandidate))
    {
       ShowMessage(screen, "Invalid candidate data. Please check all fields.", "error");
       return;
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       for (const auto& c : candidates) {
          if (c.getCandidateID() == newCandidate.getCandidateID()) {
             ShowMessage(screen, "Candidate ID already exists.", "error");
             return;
          }
       }
       candidates.push_back(newCandidate);
       saveAllCandidates(candidates);
       ShowMessage(screen,"Candidate added successfully!","success");
    } catch (const exception &e) {
       ShowMessage(screen, string("Error adding candidate: ") + e.what(), "error");
    }
}

void editCandidate(int CandidateID, const string &newName, int newPartyID, int newConstituencyID)
{
    if (!isValidCandidateID(CandidateID))
    {
       ShowMessage(screen, "Invalid candidate ID.", "error");
       return;
    }
    if (!isValidCandidateName(newName))
    {
       ShowMessage(screen, "Invalid candidate name.", "error");
       return;
    }
    if (!partyExists(newPartyID))
    {
       ShowMessage(screen, "Invalid party ID.", "error");
       return;
    }
    if (!constituencyExists(newConstituencyID))
    {
       ShowMessage(screen, "Invalid constituency ID.", "error");
       return;
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       bool found = false;
       for (auto &c : candidates)
       {
          if (c.getCandidateID() == CandidateID)
          {
             c.setCandidateName(newName);
             c.setPartyID(newPartyID);
             c.setConstituencyID(newConstituencyID);
             c.setConstituencyType(getConstituencyTypeByID(newConstituencyID));
             found = true;
             break;
          }
       }
       if (!found)
       {
          ShowMessage(screen, "Candidate ID not found.", "error");
          return;
       }
       saveAllCandidates(candidates);
       ShowMessage(screen, "Candidate updated successfully!","success");
    } catch (const exception &e) {
       ShowMessage(screen, string("Error editing candidate: ") + e.what(), "error");
    }
}

void deleteCandidateByID(int CandidateID)
{
    if (!isValidCandidateID(CandidateID))
    {
       ShowMessage(screen, "Invalid candidate ID.", "error");
       return;
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       size_t before = candidates.size();
       auto it = remove_if(candidates.begin(), candidates.end(), [CandidateID](const Candidate &c)
                       { return c.getCandidateID() == CandidateID; });
       candidates.erase(it, candidates.end());

       // Delete votes associated with this candidate
       if (CandidateID != -1) {
          deleteVotesByCandidateID(CandidateID);
       }

       saveAllCandidates(candidates);
       if (candidates.size() < before)
          ShowMessage(screen, "Candidate deleted.","success");
       else
          ShowMessage(screen, "Candidate ID not found.","error");
    } catch (const exception &e) {
       ShowMessage(screen, string("Error deleting candidate: ") + e.what(), "error");
    }
}

void listAllCandidates()
{
    try {
       vector<Candidate> list = loadAllCandidates();
       if (list.empty())
       {
          ShowMessage(screen,"No candidates found.","info");
          return;
       }
       vector<string> headers = {"ID", "Name", "Party ID", "Constituency ID", "Constituency Type"};
       vector<vector<string>> data;

       for (const auto& e : list) {
          data.push_back({
             to_string(e.getCandidateID()),
             e.getCandidateName(),
             to_string(e.getPartyID()),
             to_string(e.getConstituencyID()),
             e.getConstituencyType()
          });
       }

       ShowTableFTXUI("All Candidates", headers, data);
    } catch (const exception &e) {
       ShowMessage(screen, string("Error listing candidates: ") + e.what(), "error");
    }
}

void viewCandidatesByConstituency(int constID)
{
    if (!constituencyExists(constID))
    {
       ShowMessage(screen, "Invalid constituency ID.", "error");
       return;
    }
    try {
       vector<Candidate> list = loadAllCandidates();
       if (list.empty())
       {
          ShowMessage(screen,"No candidates found in this constituency.","info");
          return;
       }
       vector<string> headers = {"ID", "Name", "Party ID", "Constituency ID", "Constituency Type"};
       vector<vector<string>> data;
       for (const auto &c : list)
       {
          if (c.getConstituencyID() == constID)
          {
             data.push_back({
                to_string(c.getCandidateID()),
                c.getCandidateName(),
                to_string(c.getPartyID()),
                to_string(c.getConstituencyID()),
                c.getConstituencyType()
             });
          }
       }
       if (data.empty())
          ShowMessage(screen,"No candidates found in this constituency.","info");
       else
          ShowTableFTXUI("Candidates in Constituency " + to_string(constID), headers, data);
    } catch (const exception &e) {
       ShowMessage(screen, string("Error viewing candidates: ") + e.what(), "error");
    }
}

void viewCandidatesByStation(int PollingStationID)
{
    if (PollingStationID <= 0)
    {
       ShowMessage(screen, "Invalid Polling Station ID.", "error");
       return;
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       vector<PollingStation> PollingStations = loadAllStations();
       int constID1 = -1, constID2 = -1;
       for (const auto &p : PollingStations)
       {
          if (p.getPollingStationID() == PollingStationID)
          {
             constID1 = p.getConstituencyIDNA();
             constID2 = p.getConstituencyIDPA();
          }
       }
       if (constID1 == -1 && constID2 == -1) {
          ShowMessage(screen, "Polling Station not found.", "error");
          return;
       }
       vector<string> headers = {"ID", "Name", "PartyID", "Constituency ID", "Type"};
       vector<vector<string>> data;
       bool found = false;
       for (const auto &c : candidates)
       {
          if (c.getConstituencyID() == constID1 || c.getConstituencyID() == constID2)
          {
             data.push_back({
                to_string(c.getCandidateID()),
                c.getCandidateName(),
                to_string(c.getPartyID()),
                to_string(c.getConstituencyID()),
                c.getConstituencyType()
             });
             found = true;
          }
       }
       if (!found)
          ShowMessage(screen,"No candidates found for this Polling Station.","info");
       else
          ShowTableFTXUI("Candidates for Polling Station", headers, data);
    } catch (const exception &e) {
       ShowMessage(screen, string("Error viewing candidates by station: ") + e.what(), "error");
    }
}

Candidate *getCandidateByID(int CandidateID)
{
    if (!isValidCandidateID(CandidateID))
    {
       cerr << "Invalid candidate ID." << endl;
       return nullptr;
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       for (auto &c : candidates)
       {
          if (c.getCandidateID() == CandidateID)
          {
             return new Candidate(c);
          }
       }
    } catch (const exception &e) {
       cerr << "Error getting candidate by ID: " << e.what() << endl;
    }
    return nullptr;
}

string getCandidateNameByID(int CandidateID)
{
    if (!isValidCandidateID(CandidateID))
    {
       cerr << "Invalid candidate ID." << endl;
       return "";
    }
    try {
       vector<Candidate> candidates = loadAllCandidates();
       for (auto &c : candidates)
       {
          if (c.getCandidateID() == CandidateID)
          {
             return c.getCandidateName();
          }
       }
    } catch (const exception &e) {
       cerr << "Error getting candidate name by ID: " << e.what() << endl;
    }
    return "";
}

void viewCandidatesByType(string type){
    try {
       vector<Candidate> candidates = loadAllCandidates();
       vector<string> headers = {"ID", "Name", "Party ID", "Constituency ID", "Constituency Type"};
       vector<vector<string>> data;
       for (const auto &c : candidates)
       {
          if (c.getConstituencyType() == type)
          {
             data.push_back({
                to_string(c.getCandidateID()),
                c.getCandidateName(),
                to_string(c.getPartyID()),
                to_string(c.getConstituencyID()),
                c.getConstituencyType()
             });
          }
       }
       if (data.empty())
          ShowMessage(screen,"No candidates found for this type.","info");
       else
          ShowTableFTXUI("Candidates by Type", headers, data);
    } catch (const exception &e) {
       ShowMessage(screen, string("Error viewing candidates by type: ") + e.what(), "error");
    }
}

bool candidateExists(int id) {
    try {
       vector<Candidate> list = loadAllCandidates();
       for (const auto& c : list) {
          if (c.getCandidateID() == id) return true;
       }
    } catch (...) {}
    return false;
}

// --- Main Management Menu ---
void manageCandidates() {
    while (true) {
       vector<string> candidateManagement = {
          "Add Candidate",
          "View All Candidates",
          "View by Constituency",
          "Edit Candidate",
          "Delete Candidate",
          "Back"
       };

       int choice = ShowMenu(screen, "Candidate Management", candidateManagement);
       if (choice == 0) {
          string partyID_str, constID_str, provinceID_str, cityID_str;
          int partyID, constID;
          string name;

          vector<InputField> form1 = {
             {"Candidate Name", &name, InputField::TEXT}
          };
          if (!ShowForm(screen, "Add Candidate", form1)) {
             ShowMessage(screen,"Candidate creation cancelled.","error");
             continue;
          }
          if (!isValidCandidateName(name)) {
             ShowMessage(screen,"Invalid candidate name.","error");
             continue;
          }

          listAllParties();
          vector<InputField> form2 = {
             {"Party ID", &partyID_str, InputField::TEXT}
          };
          if (!ShowForm(screen, "Add Candidate", form2)) {
             ShowMessage(screen,"Candidate creation cancelled.","error");
             continue;
          }
          try {
             partyID = stoi(partyID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Party ID format.","error");
             continue;
          }
          if (!partyExists(partyID)) {
             ShowMessage(screen,"Party ID does not exist.","error");
             continue;
          }

          vector<InputField> form3 = {
             {"Select Province", &provinceID_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
          };
          if (!ShowForm(screen, "Add Candidate", form3)) {
             ShowMessage(screen,"Candidate creation cancelled.","error");
             continue;
          }
          listCitiesByProvince(provinceID_str);
          vector<InputField> form4 = {
             {"City ID", &cityID_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "Add Candidate", form4)) {
             ShowMessage(screen,"Candidate creation cancelled.","error");
             continue;
          }
          int cityID;
          try {
             cityID = stoi(cityID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid City ID format.","error");
             continue;
          }
          listConstituenciesByCity(cityID);
          vector<InputField> form5 = {
             {"Constituency ID", &constID_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "Add Candidate", form5)) {
             ShowMessage(screen,"Candidate creation cancelled.","error");
             continue;
          }
          try {
             constID = stoi(constID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!constituencyExists(constID)) {
             ShowMessage(screen,"Constituency ID does not exist.","error");
             continue;
          }

          Candidate c(getNextID("CandidateID"), name, partyID, constID, getConstituencyTypeByID(constID));
          addCandidate(c);

       } else if (choice == 1) {
          listAllCandidates();
       } else if (choice == 2) {
          string constID_str;
          vector<InputField> form = {
             {"Constituency ID", &constID_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "View Candidates by Constituency", form)) {
             ShowMessage(screen,"View cancelled.","error");
             continue;
          }
          int constID;
          try {
             constID = stoi(constID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!constituencyExists(constID)) {
             ShowMessage(screen,"Invalid Constituency ID.","error");
             continue;
          }
          viewCandidatesByConstituency(constID);
       } else if (choice == 3) {
          string id_str, name , partyID_str, constID_str;
          listAllCandidates();

          vector<InputField> form1 = {
             {"Candidate ID", &id_str, InputField::NUMBER},
             {"New Name", &name, InputField::TEXT}
          };
          if (!ShowForm(screen, "Edit Candidate", form1)) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          int id;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Candidate ID format.","error");
             continue;
          }
          if (!candidateExists(id)) {
             ShowMessage(screen,"Candidate ID not found.","info");
             continue;
          }
          if (!isValidCandidateName(name)) {
             ShowMessage(screen,"Invalid candidate name.","error");
             continue;
          }
          listAllParties();
          vector<InputField> form2 = {
             {"New Party ID", &partyID_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "Edit Candidate", form2)) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          int partyID;
          try {
             partyID = stoi(partyID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Party ID format.","error");
             continue;
          }
          if (!partyExists(partyID)) {
             ShowMessage(screen,"Party ID does not exist.","error");
             continue;
          }
          listAllConstituencies();
          vector<InputField> form3 = {
             {"New Constituency ID", &constID_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "Edit Candidate", form3)) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          int constID;
          try {
             constID = stoi(constID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!constituencyExists(constID)) {
             ShowMessage(screen,"Invalid Constituency ID.","error");
             continue;
          }
          ShowMessage(screen,"Editing Candidate ID " + to_string(id) + "...." , "info");
          editCandidate(id, name, partyID, constID);
       } else if (choice == 4) {
          string id_str;
          listAllCandidates();

          vector<InputField> form = {
             {"Candidate ID", &id_str, InputField::NUMBER}
          };
          if (!ShowForm(screen, "Delete Candidate", form)) {
             ShowMessage(screen,"Delete cancelled.","error");
             continue;
          }
          int id;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Candidate ID format.","error");
             continue;
          }
          if (!isValidCandidateID(id)) {
             ShowMessage(screen,"Invalid Candidate ID.","error");
             continue;
          }
          if (!candidateExists(id)) {
             ShowMessage(screen,"Candidate ID not found.","info");
             continue;
          }
          ShowMessage(screen,"Deleting Candidate ID " + to_string(id) + "...","info");
          deleteCandidateByID(id);
       } else if (choice == 5) {
          break;
       } else {
          ShowMessage(screen,"Invalid option.","error");
       }
    }
}