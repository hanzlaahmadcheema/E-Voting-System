
#include <custom/config.h>
#include <stdexcept>

// --- Forward Declarations ---
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

// --- PollingStation Implementation ---
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
    try {
       return PollingStation(
          j.at("PollingStationID").get<int>(),
          j.at("PollingStationName").get<string>(),
          j.at("PollingStationAddress").get<string>(),
          j.at("CityID").get<int>(),
          j.at("ConstituencyIDNA").get<int>(),
          j.at("ConstituencyIDPA").get<int>());
    } catch (const std::exception& e) {
       cerr << "Error: Invalid polling station JSON: " << e.what() << endl;
       return PollingStation();
    }
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
    return !isBlank(name) && name.length() <= 100;
}

bool isValidPollingStationAddress(const string &address) {
    return !isBlank(address) && address.length() <= 200;
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
             PollingStation ps = PollingStation::fromJSON(obj);
             if (ps.getPollingStationID() > 0)
                list.push_back(ps);
          }
       }
       catch (const std::exception& e)
       {
          cerr << "Error: Corrupted polling station data: " << e.what() << endl;
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
       cerr << "Error: Cannot open file to save polling stations." << endl;
       return;
    }
    try {
       json j;
       for (const auto &s : list)
       {
          j.push_back(s.toJSON());
       }
       file << j.dump(4);
    } catch (const std::exception& e) {
       cerr << "Error: Failed to save polling stations: " << e.what() << endl;
    }
}

// Admin: Add station
void addPollingStation(const PollingStation &s)
{
    vector<PollingStation> list = loadAllStations();

    // Validation
    if (!isValidPollingStationID(s.getPollingStationID()))
    {
       ShowMessage(screen,"Invalid Polling Station ID.","error");
       return;
    }
    if (!isValidPollingStationName(s.getPollingStationName()))
    {
       ShowMessage(screen,"Polling Station name cannot be empty or too long.","error");
       return;
    }
    if (!isValidPollingStationAddress(s.getPollingStationAddress()))
    {
       ShowMessage(screen,"Polling Station address cannot be empty or too long.","error");
       return;
    }
    if (!isValidPollingStationID(s.getConstituencyIDNA()) || !constituencyExists(s.getConstituencyIDNA()))
    {
       ShowMessage(screen,"Invalid Constituency ID (NA).","error");
       return;
    }
    if (!isValidPollingStationID(s.getConstituencyIDPA()) || !constituencyExists(s.getConstituencyIDPA()))
    {
       ShowMessage(screen,"Invalid Constituency ID (PA).","error");
       return;
    }
    if (!isValidPollingStationID(s.getCityID()))
    {
       ShowMessage(screen,"Invalid City ID.","error");
       return;
    }
    // Prevent duplicate IDs
    for (const auto& ps : list) {
       if (ps.getPollingStationID() == s.getPollingStationID()) {
          ShowMessage(screen,"Polling Station ID already exists.","error");
          return;
       }
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

    if (!isValidPollingStationID(id))
    {
       ShowMessage(screen,"Invalid Polling Station ID.","error");
       return;
    }
    if (!isValidPollingStationName(newName))
    {
       ShowMessage(screen,"New name cannot be empty or too long.","error");
       return;
    }
    if (!isValidPollingStationAddress(newAddress))
    {
       ShowMessage(screen,"New address cannot be empty or too long.","error");
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
    if (!isValidPollingStationID(id))
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
    if (!isValidPollingStationID(constID))
    {
       ShowMessage(screen,"Invalid Constituency ID.","error");
       return;
    }
    vector<PollingStation> list = loadAllStations();
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
    if (data.empty())
    {
       ShowMessage(screen,"No polling stations found for this constituency.","info");
    }
}

void listStationsByCity(int cityID)
{
    if (!isValidPollingStationID(cityID))
    {
       ShowMessage(screen,"Invalid City ID.","error");
       return;
    }
    vector<PollingStation> list = loadAllStations();
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
    if (data.empty())
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
    vector<string> headers = {"Station ID", "Name", "Address", "City ID", "Constituency ID"};
    vector<vector<string>> data;
    for (const auto &s : list)
    {
       data.push_back({
          to_string(s.getPollingStationID()),
          s.getPollingStationName(),
          s.getPollingStationAddress(),
          to_string(s.getCityID()),
          to_string(s.getConstituencyIDNA())
       });
    }
    ShowTableFTXUI("All Polling Stations.", headers, data);
}

bool pollingStationExists(int id) {
    vector<PollingStation> list = loadAllStations();
    for (const auto& s : list) {
       if (s.getPollingStationID() == id) return true;
    }
    return false;
}

void managePollingStations() {
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
          int provinceChoice = ShowMenu(screen, "Select Province", provinceMenu);
          if (provinceChoice >= 0 && provinceChoice < 4) {
             listCitiesByProvince(provinceMenu[provinceChoice]);
          } else {
             ShowMessage(screen,"Invalid province selection.","error");
             continue;
          }
          vector<InputField> form = {
             {"City ID", &cityChoice_str, InputField::TEXT}
          };
          if (!ShowForm(screen, "Create Polling Station", form)) {
             ShowMessage(screen,"Creation cancelled.","error");
             continue;
          }
          int cityChoice = 0;
          try {
             cityChoice = stoi(cityChoice_str);
          } catch (...) {
             ShowMessage(screen,"Invalid City ID format.","error");
             continue;
          }
          if (!isValidPollingStationID(cityChoice)) {
             ShowMessage(screen,"Invalid City ID.","error");
             continue;
          }
          listStationsByCity(cityChoice);

          vector<InputField> form2 = {
             {"Name", &name, InputField::TEXT},
             {"Address", &address, InputField::TEXT}
          };
          if (!ShowForm(screen, "Create Polling Station", form2)) {
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
          if (!ShowForm(screen, "Create Polling Station", form3)) {
             ShowMessage(screen,"Creation cancelled.","error");
             continue;
          }
          int ConstituencyIDNA = 0, ConstituencyIDPA = 0;
          try {
             ConstituencyIDNA = stoi(constituencyIDNA_str);
             ConstituencyIDPA = stoi(constituencyIDPA_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!isValidPollingStationID(ConstituencyIDNA) || !constituencyExists(ConstituencyIDNA)) {
             ShowMessage(screen,"Invalid Constituency ID (NA).","error");
             continue;
          }
          if (!isValidPollingStationID(ConstituencyIDPA) || !constituencyExists(ConstituencyIDPA)) {
             ShowMessage(screen,"Invalid Constituency ID (PA).","error");
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
          if (!ShowForm(screen, "Edit Polling Station", form)) {
             ShowMessage(screen,"Editing cancelled.","error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Polling Station ID format.","error");
             continue;
          }
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
          if (!ShowForm(screen, "Delete Polling Station", form)) {
             ShowMessage(screen,"Deletion cancelled.","error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Polling Station ID format.","error");
             continue;
          }
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