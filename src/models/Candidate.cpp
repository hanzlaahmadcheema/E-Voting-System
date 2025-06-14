#include <custom/config.h>


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
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

;

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

// Add any other methods or member functions as needed
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
    return !name.empty() && name.length() <= 100 && name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos;
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
        catch (const exception &e)
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
    candidates.push_back(newCandidate);
    saveAllCandidates(candidates);
    ShowMessage(screen,"Candidate added successfully!","success");
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
            c.setConstituencyType(getConstituencyTypeByID(newConstituencyID));
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
    ShowMessage(screen, "Candidate updated successfully!","success");
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

        // Delete votes associated with this candidate
    if (CandidateID != -1) {
        deleteVotesByCandidateID(CandidateID);
    }

    saveAllCandidates(candidates);
    if (candidates.size() < before)
        ShowMessage(screen, "Candidate deleted.","success");
    else
        ShowMessage(screen, "Candidate ID not found.","error");
}

// Admin: List all candidates
void listAllCandidates()
{
    vector<Candidate> list = loadAllCandidates();
    if (list.empty())
    {
        ShowMessage(screen,"No candidates found.","info");
        return;
    }
    // Using FTXUI for better output formatting
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
}

// User: View candidates in a constituency
void viewCandidatesByConstituency(int constID)
{
    if (constID <= 0)
    {
        cerr << "Invalid constituency ID.\n";
        return;
    }
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

    ShowTableFTXUI("Candidates in Constituency " + to_string(constID), headers, data);
}

void viewCandidatesByStation(int PollingStationID)
{
    int constID1, constID2;
    if (PollingStationID <= 0)
    {
        cerr << "Invalid Polling Station ID.\n";
        return;
    }
    vector<Candidate> candidates = loadAllCandidates();
    vector<PollingStation> PollingStations = loadAllStations();
    bool found = false;
        for (const auto &p : PollingStations)
    {
        if (p.getPollingStationID() == PollingStationID)
        {
            constID1 = p.getConstituencyIDNA();
            constID2 = p.getConstituencyIDPA();
        }
    }
    vector<string> headers = {"ID", "Name", "PartyID", "Constituency ID", "Type"};
    vector<vector<string>> data;
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
    {
        ShowMessage(screen,"No candidates found for this Polling Station.","info");
    }
    ShowTableFTXUI("Candidates for Polling Station", headers, data);
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

string getCandidateNameByID(int CandidateID)
{
    if (CandidateID <= 0)
    {
        cerr << "Invalid candidate ID.\n";
        return "";
    }
    vector<Candidate> candidates = loadAllCandidates();
    for (auto &c : candidates)
    {
        if (c.getCandidateID() == CandidateID)
        {
            return c.getCandidateName();
        }
    }
    return "";
}


void viewCandidatesByType(string type){
    vector<Candidate> candidates = loadAllCandidates();
    bool found = false;
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
            found = true;
        }
    }
    if (!found)
    {
        ShowMessage(screen,"No candidates found for this type.","info");
    }
    ShowTableFTXUI("Candidates by Type", headers, data);
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
        //using ftxui 
        

            vector<InputField> form1 = {
                {"Candidate Name", &name, InputField::TEXT}
            };
            bool success1 = ShowForm(screen, "Add Candidate", form1);
            if (!success1) {
                ShowMessage(screen,"[ERROR] Candidate creation cancelled.","error");
                continue;
            }
            system("cls");
            listAllParties();
            vector<InputField> form2 = {
                {"Party ID", &partyID_str, InputField::TEXT}
            };
            bool success2 = ShowForm(screen, "Add Candidate", form2);
            if (!success2) {
                ShowMessage(screen,"[ERROR] Candidate creation cancelled.","error");
                continue;
            }
            vector<InputField> form3 = {
                {"Select Province", &provinceID_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
            };
            bool success3 = ShowForm(screen, "Add Candidate", form3);
            if (!success3) {
                ShowMessage(screen,"[ERROR] Candidate creation cancelled.","error");
                continue;
            }
            listCitiesByProvince(provinceID_str);
            vector<InputField> form4 = {
                {"City ID", &cityID_str, InputField::NUMBER}
            };
            bool success4 = ShowForm(screen, "Add Candidate", form4);
            if (!success4) {
                ShowMessage(screen,"[ERROR] Candidate creation cancelled.","error");
                continue;
            }
            listConstituenciesByCity(stoi(cityID_str));
            vector<InputField> form5 = {
                {"Constituency ID", &constID_str, InputField::NUMBER}
            };
            bool success5 = ShowForm(screen, "Add Candidate", form5);
            if (!success5) {
                ShowMessage(screen,"[ERROR] Candidate creation cancelled.","error");
                continue;
            }
            partyID = stoi(partyID_str);
            constID = stoi(constID_str);

            if (isValidCandidateName(name) && partyExists(partyID) && constituencyExists(constID)) {
        Candidate c(getNextID("CandidateID"), name, partyID, constID, getConstituencyTypeByID(constID));
        addCandidate(c);
    } else {
        ShowMessage(screen,"Validation failed.","error");
    }
        } else if (choice == 1) {
            listAllCandidates();
        } else if (choice == 2) {
            string constID_str;
    
            vector<InputField> form = {
                {"Constituency ID", &constID_str, InputField::NUMBER}
            };
            bool success = ShowForm(screen, "View Candidates by Constituency", form);
            if (!success) {
                ShowMessage(screen,"[ERROR] View cancelled.","error");
                continue;
            }
            int constID = stoi(constID_str);
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
            bool success1 = ShowForm(screen, "Edit Candidate", form1);
            if (!success1) {
                ShowMessage(screen,"[ERROR] Edit cancelled.","error");
                continue;
            }
            int id = stoi(id_str);
            listAllParties();
            vector<InputField> form2 = {
                {"New Party ID", &partyID_str, InputField::NUMBER}
            };
            bool success2 = ShowForm(screen, "Edit Candidate", form2);
            if (!success2) {
                ShowMessage(screen,"[ERROR] Edit cancelled.","error");
                continue;
            }
            int partyID = stoi(partyID_str);
            listAllConstituencies();
            vector<InputField> form3 = {
                {"New Constituency ID", &constID_str, InputField::NUMBER}
            };
            bool success3 = ShowForm(screen, "Edit Candidate", form3);
            if (!success3) {
                ShowMessage(screen,"[ERROR] Edit cancelled.","error");
                continue;
            }
            int constID = stoi(constID_str);
            ShowMessage(screen,"Editing Candidate ID " << id << "...","info");
            editCandidate(id, name, partyID, constID);
        } else if (choice == 4) {
            string id_str;
            listAllCandidates();
    
            vector<InputField> form = {
                {"Candidate ID", &id_str, InputField::NUMBER}
            };
            bool success = ShowForm(screen, "Delete Candidate", form);
            if (!success) {
                ShowMessage(screen,"[ERROR] Delete cancelled.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!isValidCandidateID(id)) {
                ShowMessage(screen,"Invalid Candidate ID.","error");
                continue;
            }
            if (!candidateExists(id)) {
                ShowMessage(screen,"Candidate ID not found.","info");
                continue;
            }
            cout << "Deleting Candidate ID " << id << "...\n";
            deleteCandidateByID(id);
        } else if (choice == 5) {
            break;
        } else {
            ShowMessage(screen,"Invalid option.","error");
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