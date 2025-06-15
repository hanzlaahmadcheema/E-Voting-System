#include <custom/config.h>

extern int getNextID(const string& key);
extern bool pollingStationExists(int id);
extern bool constituencyExists(int id);
extern void listAllStations();
extern void listAllConstituencies();
extern int getVoterIDByCNIC(const string &VoterCNIC);
extern void deleteVotesByVoterID(int voterID);
extern void listCitiesByProvince(const string &province);
extern void listConstituenciesByCity(int cityID);
extern void listStationsByConstituency(int constChoice);
extern void listStationsByCity(int cityID);
extern int ShowMenu(ScreenInteractive & screen, 
     const string& heading, 
     const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                    const vector<string>& headers, 
                    const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

// Voter
Voter::Voter() : VoterID(0), VoterName(""), VoterCNIC(""), VoterGender(""), VoterAge(0), VoterAddress(""), PollingStationID(0) {}
Voter::Voter(int VoterID, const string &VoterName, const string &VoterCNIC, const string &VoterGender, int VoterAge, const string &VoterAddress, int PollingStationID)
{
    this->VoterID = VoterID;
    this->VoterName = VoterName;
    this->VoterCNIC = VoterCNIC;
    this->VoterGender = VoterGender;
    this->VoterAge = VoterAge;
    this->VoterAddress = VoterAddress;
    this->PollingStationID = PollingStationID;
}
void Voter::setVoterID(int VoterID)
{
    this->VoterID = VoterID;
}
void Voter::setVoterName(const string &VoterName)
{
    this->VoterName = VoterName;
}
void Voter::setVoterCNIC(const string &VoterCNIC)
{
    this->VoterCNIC = VoterCNIC;
}
void Voter::setVoterGender(const string &VoterGender)
{
    this->VoterGender = VoterGender;
}
void Voter::setVoterAge(int VoterAge)
{
    this->VoterAge = VoterAge;
}
void Voter::setVoterAddress(const string &VoterAddress)
{
    this->VoterAddress = VoterAddress;
}
void Voter::setPollingStationID(int PollingStationID)
{
    this->PollingStationID = PollingStationID;
}
int Voter::getVoterID() const
{
    return VoterID;
}
string Voter::getVoterName() const
{
    return VoterName;
}
string Voter::getVoterCNIC() const
{
    return VoterCNIC;
}
string Voter::getVoterGender() const
{
    return VoterGender;
}
int Voter::getVoterAge() const
{
    return VoterAge;
}
string Voter::getVoterAddress() const
{
    return VoterAddress;
}
int Voter::getPollingStationID() const
{
    return PollingStationID;
}

json Voter::toJSON() const
{
    return json{
        {"VoterID", VoterID},
        {"VoterName", VoterName},
        {"VoterCNIC", VoterCNIC},
        {"VoterGender", VoterGender},
        {"VoterAge", VoterAge},
        {"VoterAddress", VoterAddress},
        {"PollingStationID", PollingStationID}};
}

Voter Voter::fromJSON(const json &j)
{
    return Voter(
        j.at("VoterID").get<int>(),
        j.at("VoterName").get<string>(),
        j.at("VoterCNIC").get<string>(),
        j.at("VoterGender").get<string>(),
        j.at("VoterAge").get<int>(),
        j.at("VoterAddress").get<string>(),
        j.at("PollingStationID").get<int>());
}

const string VOTER_FILE = "data/voters.json";

// --- Validation Helpers ---

bool isValidCNIC(const string &VoterCNIC)
{
    if (VoterCNIC.length() != 13)
        return false;
    for (char ch : VoterCNIC)
        if (!isdigit(static_cast<unsigned char>(ch)))
            return false;
    return true;
}

bool isValidName(const string &name)
{
    if (name.empty())
        return false;
    for (char ch : name)
        if (!isalpha(static_cast<unsigned char>(ch)) && ch != ' ')
            return false;
    return true;
}

bool isValidGender(const string &gender)
{
    string g = gender;
    for (auto &c : g)
        c = tolower(c);
    return g == "male" || g == "female" || g == "other";
}

bool isValidAge(int age)
{
    return age >= 18 && age <= 120;
}

bool isValidAddress(const string &addr)
{
    return !addr.empty();
}

bool isValidID(int id)
{
    return id > 0;
}

// --- JSON Load/Save with Exception Handling ---

vector<Voter> loadAllVoters()
{
    vector<Voter> voters;
    ifstream file(VOTER_FILE);
    if (file.is_open())
    {
        try
        {
            json j;
            file >> j;
            for (auto &obj : j)
                voters.push_back(Voter::fromJSON(obj));
        }
        catch (const std::exception &e)
        {
            ShowMessage(screen, string("Error: Failed to parse voters file. ") + e.what(), "error");
        }
    }
    return voters;
}

void saveAllVoters(const vector<Voter> &voters)
{
    try
    {
        ofstream file(VOTER_FILE);
        if (!file)
            throw std::runtime_error("Could not open file for writing.");
        json j;
        for (const auto &v : voters)
            j.push_back(v.toJSON());
        file << j.dump(4);
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: Failed to save voters file. ") + e.what(), "error");
    }
}

// --- Voter CRUD ---

void registerVoter(const Voter &newVoter)
{
    try
    {
        if (!isValidID(newVoter.getVoterID()))
            throw std::invalid_argument("Invalid Voter ID.");
        if (!isValidName(newVoter.getVoterName()))
            throw std::invalid_argument("Invalid name. Only letters and spaces allowed.");
        if (!isValidCNIC(newVoter.getVoterCNIC()))
            throw std::invalid_argument("Invalid CNIC. Must be 13 digits.");
        if (!isValidGender(newVoter.getVoterGender()))
            throw std::invalid_argument("Invalid gender. Use Male, Female, or Other.");
        if (!isValidAge(newVoter.getVoterAge()))
            throw std::invalid_argument("Invalid age. Must be between 18 and 120.");
        if (!isValidAddress(newVoter.getVoterAddress()))
            throw std::invalid_argument("Address cannot be empty.");
        if (!isValidID(newVoter.getPollingStationID()))
            throw std::invalid_argument("Invalid Polling Station ID.");

        vector<Voter> voters = loadAllVoters();
        for (const auto &v : voters)
        {
            if (v.getVoterCNIC() == newVoter.getVoterCNIC())
                throw std::invalid_argument("Voter with this CNIC already exists.");
            if (v.getVoterID() == newVoter.getVoterID())
                throw std::invalid_argument("Voter with this ID already exists.");
        }
        voters.push_back(newVoter);
        saveAllVoters(voters);

        // Verify if added
        vector<Voter> updatedVoters = loadAllVoters();
        bool found = false;
        for (const auto &v : updatedVoters)
        {
            if (v.getVoterCNIC() == newVoter.getVoterCNIC())
            {
                found = true;
                break;
            }
        }
        if (found)
            ShowMessage(screen, "Voter registered successfully.", "success");
        else
            throw std::runtime_error("Failed to add voter. Please try again.");
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: ") + e.what(), "error");
    }
}

void listAllVoters()
{
    try
    {
        vector<Voter> voters = loadAllVoters();
        if (voters.empty())
        {
            ShowMessage(screen, "No voters found.", "info");
            return;
        }
        vector<string> headers = {"ID", "Name", "CNIC", "Gender", "Age", "Address", "Polling Station ID"};
        vector<vector<string>> data;
        for (const auto &v : voters)
        {
            data.push_back({
                to_string(v.getVoterID()),
                v.getVoterName(),
                v.getVoterCNIC(),
                v.getVoterGender(),
                to_string(v.getVoterAge()),
                v.getVoterAddress(),
                to_string(v.getPollingStationID())
            });
        }
        ShowTableFTXUI("Voter Details", headers, data);
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: ") + e.what(), "error");
    }
}

void editVoterByCNIC(const string &VoterCNIC, const Voter &updatedVoter)
{
    try
    {
        if (!isValidCNIC(VoterCNIC))
            throw std::invalid_argument("Invalid CNIC format.");
        vector<Voter> voters = loadAllVoters();
        bool found = false;
        for (auto &v : voters)
        {
            if (v.getVoterCNIC() == VoterCNIC)
            {
                v = updatedVoter;
                found = true;
                break;
            }
        }
        if (!found)
            throw std::invalid_argument("Voter with CNIC " + VoterCNIC + " not found.");
        saveAllVoters(voters);
        ShowMessage(screen, "Voter updated successfully.", "success");
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: ") + e.what(), "error");
    }
}

void deleteVoterByCNIC(const string &VoterCNIC)
{
    try
    {
        if (!isValidCNIC(VoterCNIC))
            throw std::invalid_argument("Invalid CNIC format.");
        vector<Voter> voters = loadAllVoters();
        size_t before = voters.size();
        int deletedVoterID = getVoterIDByCNIC(VoterCNIC);

        auto it = remove_if(voters.begin(), voters.end(), [VoterCNIC](const Voter &v)
                            { return v.getVoterCNIC() == VoterCNIC; });
        voters.erase(it, voters.end());

        if (deletedVoterID != -1)
            deleteVotesByVoterID(deletedVoterID);

        saveAllVoters(voters);
        size_t after = voters.size();

        if (after < before)
            ShowMessage(screen, "Voter deleted successfully.", "success");
        else
            throw std::invalid_argument("Voter with CNIC " + VoterCNIC + " not found.");
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: ") + e.what(), "error");
    }
}

Voter *loginByCNIC(const string &VoterCNIC)
{
    try
    {
        if (!isValidCNIC(VoterCNIC))
            throw std::invalid_argument("Invalid CNIC format.");
        vector<Voter> voters = loadAllVoters();
        for (auto &v : voters)
        {
            if (v.getVoterCNIC() == VoterCNIC)
                return new Voter(v);
        }
        throw std::invalid_argument("No voter found with this CNIC.");
    }
    catch (const std::exception &e)
    {
        ShowMessage(screen, string("Error: ") + e.what(), "error");
        return nullptr;
    }
}

int getVoterIDByCNIC(const string &VoterCNIC)
{
    try
    {
        vector<Voter> voters = loadAllVoters();
        for (const auto &v : voters)
        {
            if (v.getVoterCNIC() == VoterCNIC)
                return v.getVoterID();
        }
    }
    catch (...) {}
    return -1;
}

bool voterExists(string VoterCNIC)
{
    try
    {
        vector<Voter> list = loadAllVoters();
        for (const auto &v : list)
            if (v.getVoterCNIC() == VoterCNIC)
                return true;
    }
    catch (...) {}
    return false;
}

// --- manageVoters() remains unchanged except for improved stoi error handling ---

void manageVoters() {
    while (true) {
        vector<string> voterManagement = {
            "Register Voter",
            "View All Voters",
            "Edit Voter",
            "Delete Voter",
            "Back"
        };

        int choice = ShowMenu(screen, "Voter Management", voterManagement);

        if (choice == 0) {
            string age_str, pollingID_str, name, VoterCNIC, gender, address, cityID_str, provChoice_str;
            vector<InputField> form1 = {
                {"Voter Name", &name, InputField::TEXT},
                {"Voter CNIC", &VoterCNIC, InputField::TEXT},
                {"Voter Gender", &gender, InputField::TEXT},
                {"Voter Age", &age_str, InputField::TEXT},
                {"Voter Province", &provChoice_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
            };
            if (!ShowForm(screen, "Add Voter", form1)) {
                ShowMessage(screen, "Creation cancelled.", "error");
                continue;
            }
            int age = 0, provChoice = 0;
            try { age = stoi(age_str); provChoice = stoi(provChoice_str); }
            catch (...) { ShowMessage(screen, "Invalid numeric input.", "error"); continue; }
            if (!isValidName(name)) { ShowMessage(screen, "Invalid name. Only letters and spaces allowed.", "error"); continue; }
            if (!isValidCNIC(VoterCNIC)) { ShowMessage(screen, "Invalid CNIC format.", "error"); continue; }
            if (!isValidAge(age)) { ShowMessage(screen, "Invalid age. Age must be between 18 and 120.", "error"); continue; }
            if (!isValidGender(gender)) { ShowMessage(screen, "Invalid gender. Please enter Male, Female, or Other.", "error"); continue; }
            if (provChoice < 1 || provChoice > 4) { ShowMessage(screen, "Invalid province choice.", "error"); continue; }
            if (provChoice == 1) listCitiesByProvince("Punjab");
            else if (provChoice == 2) listCitiesByProvince("KPK");
            else if (provChoice == 3) listCitiesByProvince("Sindh");
            else if (provChoice == 4) listCitiesByProvince("Balochistan");
            vector<InputField> form2 = {
                {"City ID", &cityID_str, InputField::TEXT},
                {"Voter Address", &address, InputField::TEXT},
            };
            if (!ShowForm(screen, "Add Voter", form2)) {
                ShowMessage(screen, "Creation cancelled.", "error");
                continue;
            }
            int cityChoice = 0;
            try { cityChoice = stoi(cityID_str); }
            catch (...) { ShowMessage(screen, "Invalid City ID.", "error"); continue; }
            if (!isValidAddress(address)) { ShowMessage(screen, "Invalid address. Cannot be empty.", "error"); continue; }
            listStationsByCity(cityChoice);
            vector<InputField> form3 = {
                {"Polling ID", &pollingID_str, InputField::TEXT}
            };
            if (!ShowForm(screen, "Add Voter", form3)) {
                ShowMessage(screen, "Creation cancelled.", "error");
                continue;
            }
            int PollingID = 0;
            try { PollingID = stoi(pollingID_str); }
            catch (...) { ShowMessage(screen, "Invalid Polling Station ID.", "error"); continue; }
            if (!pollingStationExists(PollingID)) {
                ShowMessage(screen, "Invalid Polling Station ID.", "error");
                continue;
            }
            Voter v(getNextID("VoterID"), name, VoterCNIC, gender, age, address, PollingID);
            registerVoter(v);
        } else if (choice == 1) {
            listAllVoters();
        } else if (choice == 2) {
            string age_str, pollingID_str, name, VoterCNIC, gender, address, cityID_str, provChoice_str;
            vector<InputField> form1 = {
                {"Voter CNIC", &VoterCNIC, InputField::TEXT}
            };
            if (!ShowForm(screen, "Edit Voter", form1)) {
                ShowMessage(screen, "Edition cancelled.", "error");
                continue;
            }
            if (!voterExists(VoterCNIC)) { ShowMessage(screen, "Voter with this CNIC does not exist.", "error"); continue; }
            if (!isValidCNIC(VoterCNIC)) { ShowMessage(screen, "Invalid CNIC format.", "error"); continue; }
            vector<InputField> form2 = {
                {"Voter Name", &name, InputField::TEXT},
                {"Voter CNIC", &VoterCNIC, InputField::TEXT},
                {"Voter Gender", &gender, InputField::TEXT},
                {"Voter Age", &age_str, InputField::TEXT},
                {"Voter Province", &provChoice_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
            };
            if (!ShowForm(screen, "Edit Voter", form2)) {
                ShowMessage(screen, "Edition cancelled.", "error");
                continue;
            }
            int age = 0, provChoice = 0;
            try { age = stoi(age_str); provChoice = stoi(provChoice_str); }
            catch (...) { ShowMessage(screen, "Invalid numeric input.", "error"); continue; }
            if (!isValidName(name)) { ShowMessage(screen, "Invalid name. Only letters and spaces allowed.", "error"); continue; }
            if (!isValidCNIC(VoterCNIC)) { ShowMessage(screen, "Invalid CNIC format.", "error"); continue; }
            if (!isValidAge(age)) { ShowMessage(screen, "Invalid age. Age must be between 18 and 120.", "error"); continue; }
            if (!isValidGender(gender)) { ShowMessage(screen, "Invalid gender. Please enter Male, Female, or Other.", "error"); continue; }
            if (provChoice < 1 || provChoice > 4) { ShowMessage(screen, "Invalid province choice.", "error"); continue; }
            if (provChoice == 1) listCitiesByProvince("Punjab");
            else if (provChoice == 2) listCitiesByProvince("KPK");
            else if (provChoice == 3) listCitiesByProvince("Sindh");
            else if (provChoice == 4) listCitiesByProvince("Balochistan");
            vector<InputField> form3 = {
                {"City ID", &cityID_str, InputField::TEXT},
                {"Voter Address", &address, InputField::TEXT},
            };
            if (!ShowForm(screen, "Edit Voter", form3)) {
                ShowMessage(screen, "Edition cancelled.", "error");
                continue;
            }
            int cityChoice = 0;
            try { cityChoice = stoi(cityID_str); }
            catch (...) { ShowMessage(screen, "Invalid City ID.", "error"); continue; }
            if (!isValidAddress(address)) { ShowMessage(screen, "Invalid address. Cannot be empty.", "error"); continue; }
            listStationsByCity(cityChoice);
            vector<InputField> form4 = {
                {"Polling ID", &pollingID_str, InputField::TEXT}
            };
            if (!ShowForm(screen, "Edit Voter", form4)) {
                ShowMessage(screen, "Edition cancelled.", "error");
                continue;
            }
            int PollingID = 0;
            try { PollingID = stoi(pollingID_str); }
            catch (...) { ShowMessage(screen, "Invalid Polling Station ID.", "error"); continue; }
            if (!pollingStationExists(PollingID)) {
                ShowMessage(screen, "Invalid Polling Station ID.", "error");
                continue;
            }
            editVoterByCNIC(VoterCNIC, Voter(getVoterIDByCNIC(VoterCNIC), name, VoterCNIC, gender, age, address, PollingID));
        } else if (choice == 3) {
            string VoterCNIC;
            vector<InputField> form1 = {
                {"Voter CNIC", &VoterCNIC, InputField::TEXT}
            };
            if (!ShowForm(screen, "Delete Voter", form1)) {
                ShowMessage(screen, "Deletion cancelled.", "error");
                continue;
            }
            if (!voterExists(VoterCNIC)) { ShowMessage(screen, "Voter with this CNIC does not exist.", "error"); continue; }
            if (!isValidCNIC(VoterCNIC)) { ShowMessage(screen, "Invalid CNIC format.", "error"); continue; }
            deleteVoterByCNIC(VoterCNIC);
        } else if (choice == 4) {
            break;
        } else {
            ShowMessage(screen, "Invalid option. Please try again.", "error");
        }
    }
}
