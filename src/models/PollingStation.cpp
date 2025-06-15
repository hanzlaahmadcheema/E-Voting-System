
#include <custom/config.h>

extern int getNextID(const string& key);
extern bool constituencyExists(int id);
extern void listAllConstituencies();
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

// PollingStation
PollingStation::PollingStation() : PollingStationID(0), PollingStationName(""), PollingStationAddress(""), CityID(0), ConstituencyIDNA(0), ConstituencyIDPA(0) {}
PollingStation::PollingStation(int PollingStationID, const string &PollingStationName, const string &PollingStationAddress, int CityID, int ConstituencyIDNA, int ConstituencyIDPA)
{
    this->PollingStationID = PollingStationID;
    this->PollingStationName = PollingStationName;
    this->PollingStationAddress = PollingStationAddress;
    this->CityID = CityID;
    this->ConstituencyIDNA = ConstituencyIDNA;
    this->ConstituencyIDPA = ConstituencyIDPA;
}
void PollingStation::setPollingStationID(int PollingStationID)
{
    this->PollingStationID = PollingStationID;
}
void PollingStation::setPollingStationName(const string &PollingStationName)
{
    this->PollingStationName = PollingStationName;
}
void PollingStation::setPollingStationAddress(const string &PollingStationAddress)
{
    this->PollingStationAddress = PollingStationAddress;
}
void PollingStation::setCityID(int CityID)
{
    this->CityID = CityID;
}
void PollingStation::setConstituencyIDNA(int ConstituencyIDNA)
{
    this->ConstituencyIDNA = ConstituencyIDNA;
}
void PollingStation::setConstituencyIDPA(int ConstituencyIDPA)
{
    this->ConstituencyIDPA = ConstituencyIDPA;
}
int PollingStation::getPollingStationID() const
{
    return PollingStationID;
}
string PollingStation::getPollingStationName() const
{
    return PollingStationName;
}
string PollingStation::getPollingStationAddress() const
{
    return PollingStationAddress;
}
int PollingStation::getConstituencyIDNA() const
{
    return ConstituencyIDNA;
}
int PollingStation::getCityID() const
{
    return CityID;
}
int PollingStation::getConstituencyIDPA() const
{
    return ConstituencyIDPA;
}

json PollingStation::toJSON() const
{
    return json{
        {"PollingStationID", PollingStationID},
        {"PollingStationName", PollingStationName},
        {"PollingStationAddress", PollingStationAddress},
        {"CityID", CityID},
        {"ConstituencyIDNA", ConstituencyIDNA},
        {"ConstituencyIDPA", ConstituencyIDPA}};
}

PollingStation PollingStation::fromJSON(const json &j)
{
    return PollingStation(
        j.at("PollingStationID").get<int>(),
        j.at("PollingStationName").get<string>(),
        j.at("PollingStationAddress").get<string>(),
        j.at("CityID").get<int>(),
        j.at("ConstituencyIDNA").get<int>(),
        j.at("ConstituencyIDPA").get<int>());
}

const string STATION_FILE = "data/polling_stations.json";

// Helper: Check if string is empty or whitespace
bool isBlank(const string &str)
{
    return str.find_first_not_of(" \t\n\r") == string::npos;
}

bool isValidPollingStationID(int id) {
    return id > 0;
}

bool isValidPollingStationName(const string &name) {
    return !name.empty() && name.length() <= 100;
}

bool isValidPollingStationAddress(const string &address) {
    return !address.empty() && address.length() <= 200;
}

// Load all stations
vector<PollingStation> loadAllStations()
{
    vector<PollingStation> list;
    ifstream file(STATION_FILE);
    if (file.is_open())
    {
        json j;
        try
        {
            file >> j;
            for (auto &obj : j)
            {
                list.push_back(PollingStation::fromJSON(obj));
            }
        }
        catch (...)
        {
            cerr << "Error: Corrupted polling station data.";
        }
    }
    return list;
}

// Save all stations
void saveAllStations(const vector<PollingStation> &list)
{
    ofstream file(STATION_FILE);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file to save polling stations.";
        return;
    }
    json j;
    for (const auto &s : list)
    {
        j.push_back(s.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add station
void addPollingStation(const PollingStation &s)
{
    vector<PollingStation> list = loadAllStations();

    // Validation
    if (s.getPollingStationID() <= 0)
    {
        ShowMessage(screen,"Invalid Polling Station ID.","error");
        return;
    }
    if (isBlank(s.getPollingStationName()))
    {
        ShowMessage(screen,"Polling Station name cannot be empty.","error");
        return;
    }
    if (isBlank(s.getPollingStationAddress()))
    {
        ShowMessage(screen,"Polling Station address cannot be empty.","error");
        return;
    }
    if (s.getConstituencyIDNA() <= 0)
    {
        ShowMessage(screen,"Invalid Constituency ID.","error");
        return;
    }
    if (s.getConstituencyIDPA() <= 0)
    {
        ShowMessage(screen,"Invalid Constituency ID.","error");
        return;
    }
    list.push_back(s);
    saveAllStations(list);
    ShowMessage(screen,"Polling station added.","success");
}

// Admin: Edit station
void editPollingStation(int id, const string &newName, const string &newAddress)
{
    vector<PollingStation> list = loadAllStations();
    bool found = false;

    if (id <= 0)
    {
        ShowMessage(screen,"Invalid Polling Station ID.","error");
        return;
    }
    if (isBlank(newName))
    {
        ShowMessage(screen,"New name cannot be empty.","error");
        return;
    }
    if (isBlank(newAddress))
    {
        ShowMessage(screen,"New address cannot be empty.","error");
        return;
    }

    for (auto &s : list)
    {
        if (s.getPollingStationID() == id)
        {
            s.setPollingStationName(newName);
            s.setPollingStationAddress(newAddress);
            found = true;
            break;
        }
    }
    if (!found)
    {
        ShowMessage(screen,"Polling station not found.","error");
        return;
    }
    saveAllStations(list);
    ShowMessage(screen,"Polling station updated.","success");
}

// Admin: Delete station
void deletePollingStation(int id)
{
    if (id <= 0)
    {
        ShowMessage(screen,"Invalid Polling Station ID.","error");
        return;
    }
    vector<PollingStation> list = loadAllStations();
    auto it = remove_if(list.begin(), list.end(), [id](const PollingStation &s)
                        { return s.getPollingStationID() == id; });
    if (it == list.end())
    {
        ShowMessage(screen,"Polling station not found.","error");
        return;
    }
    list.erase(it, list.end());
    saveAllStations(list);
    ShowMessage(screen,"Polling station deleted.","success");
}

PollingStation getPollingStationByID(int id) {
    vector<PollingStation> list = loadAllStations();
    for (const auto& s : list) {
        if (s.getPollingStationID() == id) return s;
    }
    return PollingStation();
}
// Admin/User: View all stations by constituency
void listStationsByConstituency(int constID)
{
    if (constID <= 0)
    {
        ShowMessage(screen,"Invalid Constituency ID.","error");
        return;
    }
    vector<PollingStation> list = loadAllStations();
    bool found = false;
    
    vector<string> headers = {"Station ID", "Name", "Address", "City ID", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &s : list)
    {
        if (s.getConstituencyIDNA() == constID || s.getConstituencyIDPA() == constID)
        {
            data.push_back({
                to_string(s.getPollingStationID()),
                s.getPollingStationName(),
                s.getPollingStationAddress(),
                to_string(s.getCityID()),
                to_string(s.getConstituencyIDNA())
            });
        }
    }
    ShowTableFTXUI("Polling Stations in Constituency " + to_string(constID), headers, data);
    if (!found)
    {
        ShowMessage(screen,"No polling stations found for this constituency.","info");
    }

}

void listStationsByCity(int cityID)
{
    if (cityID <= 0)
    {
        ShowMessage(screen,"Invalid City ID.","error");
        return;
    }
    vector<PollingStation> list = loadAllStations();
    bool found = false;
    
    vector<string> headers = {"Station ID", "Name", "Address", "City ID", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &s : list)
    {
        if (s.getCityID() == cityID)
        {
            data.push_back({
                to_string(s.getPollingStationID()),
                s.getPollingStationName(),
                s.getPollingStationAddress(),
                to_string(s.getCityID()),
                to_string(s.getConstituencyIDNA())
            });
        }
    }
    ShowTableFTXUI("Polling Stations in City " + to_string(cityID), headers, data);
    if (!found)
    {
        ShowMessage(screen,"No polling stations found for this city.","info");
    }
}

void listAllStations()
{
    vector<PollingStation> list = loadAllStations();
    if (list.empty())
    {
        ShowMessage(screen,"ℹNo polling stations found.","info");
        return;
    }
    bool found = false;
    vector<string> headers = {"Station ID", "Name", "Address", "City ID", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &s : list)
    {
        {
            data.push_back({
                to_string(s.getPollingStationID()),
                s.getPollingStationName(),
                s.getPollingStationAddress(),
                to_string(s.getCityID()),
                to_string(s.getConstituencyIDNA())
            });
    }
    }
        ShowTableFTXUI("AllPolling Stations.", headers, data);
    if (!found)
    {
        ShowMessage(screen,"No polling stations found.","info");
    }
}

bool pollingStationExists(int id) {
    vector<PollingStation> list = loadAllStations();
    for (const auto& s : list) {
        if (s.getPollingStationID() == id) return true;
    }
    return false;
}

void managePollingStations() {
    int choice;
    while (true) {
   

    vector<string> pollingStationMenu = {
        "Add Polling Station",
        "View All Polling Stations",
        "Edit Polling Station",
        "Delete Polling Station",
        "Back"
    };

    int choice = ShowMenu(screen, "Polling Station Management", pollingStationMenu);
        if (choice == 0) {
            string cityChoice_str, name, address, constituencyIDNA_str, constituencyIDPA_str;
    
            vector<string> provinceMenu = {
                "Punjab",
                "KPK",
                "Sindh",
                "Balochistan"
            };
            int choice = ShowMenu(screen, "Select Province", provinceMenu);
            if (choice == 1) {
                listCitiesByProvince("Punjab");
            } else if (choice == 2) {
                listCitiesByProvince("KPK");
            } else if (choice == 3) {
                listCitiesByProvince("Sindh");
            } else if (choice == 4) {
                listCitiesByProvince("Balochistan");
            }
            vector<InputField> form = {
                {"City ID", &cityChoice_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Create Polling Station", form);
            if (!success) {
                  ShowMessage(screen,"Creation cancelled.","error");
                continue;
            }
            int cityChoice = stoi(cityChoice_str);
            listStationsByCity(cityChoice);
            vector<InputField> form2 = {
                {"Name", &name, InputField::TEXT},
                {"Address", &address, InputField::TEXT}
            };
            bool success2 = ShowForm(screen, "Create Polling Station", form2);
            if (!success2) {
                  ShowMessage(screen,"Creation cancelled.","error");
                continue;
            }
            if (!isValidPollingStationName(name)) {
                  ShowMessage(screen,"Invalid Polling Station Name.","error");
                continue;
            }
            if (!isValidPollingStationAddress(address)) {
                  ShowMessage(screen,"Invalid Polling Station Address.","error");
                continue;
            }
            listConstituenciesByCity(cityChoice);
            vector<InputField> form3 = {
                {"Constituency ID2", &constituencyIDNA_str, InputField::TEXT},
                {"Constituency ID1", &constituencyIDPA_str, InputField::TEXT}
            };
            bool success3 = ShowForm(screen, "Create Polling Station", form3);
            if (!success3) {
                  ShowMessage(screen,"Creation cancelled.","error");
                continue;
            }
            int ConstituencyIDNA = stoi(constituencyIDNA_str);
            int ConstituencyIDPA = stoi(constituencyIDPA_str);

            if (!constituencyExists(ConstituencyIDNA)) {
                  ShowMessage(screen,"Invalid Constituency ID.","error");
                continue;
            }
            if (!constituencyExists(ConstituencyIDPA)) {
                  ShowMessage(screen,"Invalid Constituency ID.","error");
                continue;
            }
            PollingStation ps(getNextID("PollingStationID"), name, address, cityChoice, ConstituencyIDNA, ConstituencyIDPA);
            addPollingStation(ps);
        } else if (choice == 1) {
            listAllStations();
        } else if (choice == 2) {
            string id_str, name, address;
            listAllStations();
    
            vector<InputField> form = {
                {"Station ID", &id_str, InputField::TEXT},
                {"Station Name", &name, InputField::TEXT},
                {"Station Address", &address, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Edit Polling Station", form);
            if (!success) {
                  ShowMessage(screen,"Editing cancelled.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!isValidPollingStationID(id)) {
                  ShowMessage(screen,"Invalid Polling Station ID.","error");
                continue;
            }
            if (!pollingStationExists(id)) {
                  ShowMessage(screen,"Polling Station ID not found.","error");
                continue;
            }
            if (!isValidPollingStationName(name)) {
                  ShowMessage(screen,"Invalid Polling Station Name.","error");
                continue;
            }
            if (!isValidPollingStationAddress(address)) {
                  ShowMessage(screen,"Invalid Polling Station Address.","error");
                continue;
            }
            editPollingStation(id, name, address);
        } else if (choice == 3) {
            string id_str;
            listAllStations();
    
            vector<InputField> form = {
                {"Station ID", &id_str, InputField::TEXT}
            };
            bool success = ShowForm(screen, "Delete Polling Station", form);
            if (!success) {
                  ShowMessage(screen,"Deletion cancelled.","error");
                continue;
            }
            int id = stoi(id_str);
            if (!isValidPollingStationID(id)) {
                  ShowMessage(screen,"Invalid Polling Station ID.","error");
                continue;
            }
            if (!pollingStationExists(id)) {
                  ShowMessage(screen,"Polling Station ID not found.","error");
                continue;
            }
            deletePollingStation(id);
        } else if (choice == 4) {
            break;
        } else {
              ShowMessage(screen,"Invalid option. Please try again.","error");
        }
    }
}
// int main()
// {
//     // Example usage
//     PollingStation ps(getNextID("PollingStationID"), "Main Street Station", "123 Main St", 101);
//     addPollingStation(ps);
//     listStationsByConstituency(101);
//     editPollingStation(ps.getPollingStationID(), "Updated Station", "456 Updated St");
//     listStationsByConstituency(101);
//     deletePollingStation(ps.getPollingStationID());
//     listStationsByConstituency(101);
//     return 0;
// }