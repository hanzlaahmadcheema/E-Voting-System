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

;

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

bool isValidCNIC(const string &VoterCNIC)
{
    if (VoterCNIC.length() != 13)
        return false;
    for (char ch : VoterCNIC)
    {
        if (!isdigit(ch))
            return false;
    }
    return true;
}

// Helper: Validate name (non-empty, letters and spaces)
bool isValidName(const string &name)
{
    if (name.empty() && name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos)
        return false;
    for (char ch : name)
    {
        if (!isalpha(ch) && ch != ' ')
            return false;
    }
    return true;
}

// Helper: Validate gender (Male/Female/Other)
bool isValidGender(const string &gender)
{
    string g = gender;
    for (auto &c : g)
        c = tolower(c);
    return g == "male" || g == "female" || g == "other";
}

// Helper: Validate age (18-120)
bool isValidAge(int age)
{
    return age >= 18 && age <= 120;
}

// Helper: Validate address (non-empty)
bool isValidAddress(const string &addr)
{
    return !addr.empty();
}

// Helper: Validate IDs (positive)
bool isValidID(int id)
{
    return id > 0;
}

// Load all voters from JSON file
vector<Voter> loadAllVoters()
{
    vector<Voter> voters;
    ifstream file(VOTER_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                voters.push_back(Voter::fromJSON(obj));
            }
        }
        catch (...)
        {
            ShowMessage(screen,"Error: Failed to parse voters file.","error");
        }
    }
    return voters;
}

// Save all voters to JSON file
void saveAllVoters(const vector<Voter> &voters)
{
    ofstream file(VOTER_FILE);
    json j;
    for (const auto &v : voters)
    {
        j.push_back(v.toJSON());
    }
    file << j.dump(4);
}

// Admin: Register new voter (with duplicate CNIC check and validations)
void registerVoter(const Voter &newVoter)
{
    // Field validations
    if (!isValidID(newVoter.getVoterID()))
    {
        ShowMessage(screen,"Error: Invalid Voter ID.", "error");
        return;
    }
    if (!isValidName(newVoter.getVoterName()))
    {
        ShowMessage(screen,"Error: Invalid name. Only letters and spaces allowed.", "error");
        return;
    }
    if (!isValidCNIC(newVoter.getVoterCNIC()))
    {
        ShowMessage(screen,"Error: Invalid CNIC. Must be 13 digits.", "error");
        return;
    }
    if (!isValidGender(newVoter.getVoterGender()))
    {
        ShowMessage(screen,"Error: Invalid gender. Use Male, Female, or Other.", "error");
        return;
    }
    if (!isValidAge(newVoter.getVoterAge()))
    {
        ShowMessage(screen,"Error: Invalid age. Must be between 18 and 120.", "error");
        return;
    }
    if (!isValidAddress(newVoter.getVoterAddress()))
    {
        ShowMessage(screen,"Error: Address cannot be empty.", "error");
        return;
    }
    if (!isValidID(newVoter.getPollingStationID()))
    {
        ShowMessage(screen,"Error: Invalid Polling Station ID.", "error");
        return;
    }

    vector<Voter> voters = loadAllVoters();
    // Check for duplicate CNIC
    for (const auto &v : voters)
    {
        if (v.getVoterCNIC() == newVoter.getVoterCNIC())
        {
            ShowMessage(screen,"Error: Voter with this CNIC already exists.", "error");
            return;
        }
        if (v.getVoterID() == newVoter.getVoterID())
        {
            ShowMessage(screen,"Error: Voter with this ID already exists.", "error");
            return;
        }
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
    {
        ShowMessage(screen,"Voter registered successfully.", "success");
    }
    else
    {
        ShowMessage(screen,"Error: Failed to add voter. Please try again.", "error");
    }
}

// Admin: List all voters
void listAllVoters()
{
    vector<Voter> voters = loadAllVoters();
    if (voters.empty())
    {
        ShowMessage(screen,"No voters found.","info");
        return;
    }
    for (const auto &v : voters)
    {
        // 
        // vector<string> headers = {"ID", "Name", "CNIC", "Gender", "Age", "Address", "Polling Station ID"};
        // vector<vector<string>> data;
        //     data.push_back({
        //         to_string(v.getVoterID()),
        //         v.getVoterName(),
        //         v.getVoterCNIC(),
        //         v.getVoterGender(),
        //         to_string(v.getVoterAge()),
        //         v.getVoterAddress(),
        //         to_string(v.getPollingStationID())
        //     });
        // ShowTableFTXUI("Voter Details", headers, data);
    }
}

// Admin: Edit voter by CNIC (with check)
void editVoterByCNIC(const string &VoterCNIC, const Voter &updatedVoter)
{
    if (!isValidCNIC(VoterCNIC))
    {
        ShowMessage(screen,"Error: Invalid CNIC format.", "error");
        return;
    }
    vector<Voter> voters = loadAllVoters();
    bool found = false;
    for (auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            v = updatedVoter; // Update the voter
            found = true;
            break;
        }
    }
    if (!found)
    {
        ShowMessage(screen,"Error: Voter with CNIC " + VoterCNIC + " not found.", "error");
        return;
    }
    saveAllVoters(voters);
    ShowMessage(screen,"Voter updated successfully.", "success");
}

void deleteVoterByCNIC(const string &VoterCNIC)
{
    if (!isValidCNIC(VoterCNIC))
    {
        ShowMessage(screen,"Error: Invalid CNIC format.", "error");
        return;
    }
    vector<Voter> voters = loadAllVoters();
    size_t before = voters.size();
    int deletedVoterID = getVoterIDByCNIC(VoterCNIC);

    // Remove the voter from the list
    auto it = remove_if(voters.begin(), voters.end(), [VoterCNIC](const Voter &v)
                        { return v.getVoterCNIC() == VoterCNIC; });
    voters.erase(it, voters.end());

    // Delete votes associated with this voter
    if (deletedVoterID != -1) {
        deleteVotesByVoterID(deletedVoterID);
    }

    saveAllVoters(voters);
    size_t after = voters.size();

    if (after < before)
    {
        ShowMessage(screen,"Voter deleted successfully.", "success");
    }
    else
    {
        ShowMessage(screen,"Error: Voter with CNIC " + VoterCNIC + " not found.", "error");
    }
}

// User: Login by CNIC (returns voter object or null, with check)
Voter *loginByCNIC(const string &VoterCNIC)
{
    if (!isValidCNIC(VoterCNIC))
    {
        ShowMessage(screen,"Error: Invalid CNIC format.", "error");
        return nullptr;
    }
    vector<Voter> voters = loadAllVoters();
    for (auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            return new Voter(v);
        }
    }
    ShowMessage(screen,"Error: No voter found with this CNIC.", "error");
    return nullptr;
}

//function to get ID from Voter CNIC
int getVoterIDByCNIC(const string &VoterCNIC)
{
    vector<Voter> voters = loadAllVoters();
    for (const auto &v : voters)
    {
        if (v.getVoterCNIC() == VoterCNIC)
        {
            return v.getVoterID();
        }
    }
    return -1; // Not found
}

bool voterExists(string VoterCNIC) {
    vector<Voter> list = loadAllVoters();
    for (const auto& v : list) {
        if (v.getVoterCNIC() == VoterCNIC) return true;
    }
    return false;
}

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
            bool success1 = ShowForm(screen, "Add Voter", form1);
            if (!success1) {
                ShowMessage(screen,"Creation cancelled.", "error");
                continue;
            }
            int age = stoi(age_str);
            int provChoice = stoi(provChoice_str);
            if (!isValidName(name)) {
                ShowMessage(screen,"Invalid name. Only letters and spaces allowed.", "error");
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                ShowMessage(screen,"Invalid CNIC format.", "error");
                continue;
            }
            if (!isValidAge(age)) {
                ShowMessage(screen,"Invalid age. Age must be positive or greater than 18."," error");
                continue;
            }
            if (!isValidGender(gender)) {
                ShowMessage(screen,"Invalid gender. Please enter Male or Female.", "error");
                continue;
            }
            if (provChoice < 1 || provChoice > 4) {
                ShowMessage(screen,"Invalid province choice.", "error");
                continue;
            }
            if (provChoice == 1) {
                listCitiesByProvince("Punjab");
            } else if (provChoice == 2) {
                listCitiesByProvince("KPK");
            } else if (provChoice == 3) {
                listCitiesByProvince("Sindh");
            } else if (provChoice == 4) {
                listCitiesByProvince("Balochistan");
            }
            vector<InputField> form2 = {
                {"City ID", &cityID_str, InputField::TEXT},
                {"Voter Address", &address, InputField::TEXT},
                };
            bool success2 = ShowForm(screen, "Add Voter", form2);
            if (!success2) {
                ShowMessage(screen,"Creation cancelled.", "error");
                continue;
            }
            int cityChoice = stoi(cityID_str);
            if (!isValidAddress(address)) {
                ShowMessage(screen,"Invalid address. Only letters, numbers, and spaces allowed.", "error");
                continue;
            }
            listStationsByCity(cityChoice);
            vector<InputField> form3 = {
                {"Polling ID", &pollingID_str, InputField::TEXT}
                };
            bool success3 = ShowForm(screen, "Add Voter", form3);
            if (!success3) {
                ShowMessage(screen,"Creation cancelled.", "error");
                continue;
            }
            int PollingID = stoi(pollingID_str);
            if (!pollingStationExists(PollingID)) {
                ShowMessage(screen,"Invalid Polling Station ID.", "error");
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
            bool success1 = ShowForm(screen, "Edit Voter", form1);
            if (!success1) {
                ShowMessage(screen,"Edition cancelled.", "error");
                continue;
            }
            int age = stoi(age_str);
            int provChoice = stoi(provChoice_str);
            if (!voterExists(VoterCNIC)) {
                ShowMessage(screen,"Voter with this CNIC does not exist.", "error");
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                ShowMessage(screen,"Invalid CNIC format.", "error");
                continue;
            }
            vector<InputField> form2 = {
                {"Voter Name", &name, InputField::TEXT},
                {"Voter CNIC", &VoterCNIC, InputField::TEXT},
                {"Voter Gender", &gender, InputField::TEXT},
                {"Voter Age", &age_str, InputField::TEXT},
                {"Voter Province", &provChoice_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
                };
            bool success2 = ShowForm(screen, "Edit Voter", form2);
            if (!success2) {
                ShowMessage(screen,"Edition cancelled.", "error");
                continue;
            }
            if (!isValidName(name)) {
                ShowMessage(screen,"Invalid name. Only letters and spaces allowed.", "error");
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                ShowMessage(screen,"Invalid CNIC format.", "error");
                continue;
            }
            if (!isValidAge(age)) {
                ShowMessage(screen,"Invalid age. Age must be positive or greater than 18.", "error");
                continue;
            }
            if (!isValidGender(gender)) {
                ShowMessage(screen,"Invalid gender. Please enter Male or Female.", "error");
                continue;
            }
            if (provChoice < 1 || provChoice > 4) {
                ShowMessage(screen,"Invalid province choice.", "error");
                continue;
            }
            if (provChoice == 1) {
                listCitiesByProvince("Punjab");
            } else if (provChoice == 2) {
                listCitiesByProvince("KPK");
            } else if (provChoice == 3) {
                listCitiesByProvince("Sindh");
            } else if (provChoice == 4) {
                listCitiesByProvince("Balochistan");
            }
            vector<InputField> form3 = {
                {"City ID", &cityID_str, InputField::TEXT},
                {"Voter Address", &address, InputField::TEXT},
                };
            bool success3 = ShowForm(screen, "Edit Voter", form3);
            if (!success3) {
                ShowMessage(screen,"Edition cancelled.", "error");
                continue;
            }
            int cityChoice = stoi(cityID_str);
            if (!isValidAddress(address)) {
                ShowMessage(screen,"Invalid address. Only letters, numbers, and spaces allowed.", "error");
                continue;
            }
            listStationsByCity(cityChoice);
            vector<InputField> form4 = {
                {"Polling ID", &pollingID_str, InputField::TEXT}
                };
            bool success4 = ShowForm(screen, "Edit Voter", form4);
            if (!success4) {
                ShowMessage(screen,"Edition cancelled.", "error");
                continue;
            }
            int PollingID = stoi(pollingID_str);
            if (!pollingStationExists(PollingID)) {
                ShowMessage(screen,"Invalid Polling Station ID.", "error");
                continue;
            }
            editVoterByCNIC(VoterCNIC, Voter(getVoterIDByCNIC(VoterCNIC), name, VoterCNIC, gender, age, address, PollingID));
        } else if (choice == 3) {
            string VoterCNIC;
    
            vector<InputField> form1 = {
                {"Voter CNIC", &VoterCNIC, InputField::TEXT}
                };
            bool success1 = ShowForm(screen, "Delete Voter", form1);
            if (!success1) {
                ShowMessage(screen,"Deletion cancelled.", "error");
                continue;
            }
            if (!voterExists(VoterCNIC)) {
                ShowMessage(screen,"Voter with this CNIC does not exist.", "error");
                continue;
            }
            if (!isValidCNIC(VoterCNIC)) {
                ShowMessage(screen,"Invalid CNIC format.", "error");
                continue;
            }
            deleteVoterByCNIC(VoterCNIC);
        } else if (choice == 4) {
            break;
        } else {
            ShowMessage(screen,"Invalid option. Please try again.", "error");
        }
    }
}


// int main()
// {
//     // Example usage
//     Voter v1(getNextID("VoterID"), "John Doe", "1234567890123", "Male", 30, "123 Main St", 101, 1);
//     registerVoter(v1);
//     listAllVoters();
//     editVoterByCNIC(v1.getVoterID(), Voter(v1.getVoterID(), "John Doe", "1234567890123", "Male", 31, "123 Main St", 101, 1));
//     listAllVoters();
//     deleteVoterByCNIC(v1.getVoterID());
//     listAllVoters();
//     Voter *loggedInVoter = loginByCNIC("1234567890123");
//     if (loggedInVoter)
//     {
//         viewProfile(*loggedInVoter);
//         delete loggedInVoter; // Clean up
//     }
//     return 0;
// }