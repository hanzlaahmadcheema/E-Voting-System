#include <custom/config.h>

// Externs
extern bool cityExists(int id);
extern bool electionExists(int id);
extern void listAllCities();
extern void listAllElections();
extern void listCitiesByProvince(const string &province);
extern string getElectionTypeByID(int id);
extern string toLower(const string& str);
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
    try {
       return Constituency(
          j.at("ConstituencyID").get<int>(),
          j.at("ConstituencyName").get<string>(),
          j.at("CityID").get<int>(),
          j.at("ElectionID").get<int>());
    } catch (const std::exception& e) {
       cerr << "Error: Invalid JSON format for constituency: " << e.what() << endl;
       return Constituency();
    }
}

const string CONSTITUENCY_FILE = "D://E-Voting-System/data/constituencies.json";

bool constituencyNameExists(const vector<Constituency> &list, const string &name)
{
   std::unordered_map<std::string, bool> nameMap;
   for (const auto &p : list)
   {
      nameMap[toLower(p.getConstituencyName())] = true;
   }
   return nameMap.find(toLower(name)) != nameMap.end();
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
             Constituency c = Constituency::fromJSON(obj);
             if (c.getConstituencyID() > 0) // Only add valid
                list.push_back(c);
          }
       }
       catch (const std::exception& e)
       {
          cerr << "Error: Invalid JSON format in constituency file: " << e.what() << endl;
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
       cerr << "Error: Unable to open file for writing." << endl;
       return;
    }
    json j;
    for (const auto &c : list)
    {
       j.push_back(c.toJSON());
    }
    try {
       file << j.dump(4);
    } catch (const std::exception& e) {
       cerr << "Error: Failed to write JSON to file: " << e.what() << endl;
    }
}

bool isValidConstituencyIDNA(int ConstituencyIDNA, int cityChoice){
    vector<Constituency> list = loadAllConstituencies();
      for (const auto &c : list)
      {
         if (c.getConstituencyID() == ConstituencyIDNA && c.getCityID() == cityChoice)
            return true;
      }
      return false;
}

bool isValidConstituencyIDPA(int ConstituencyIDPA, int cityChoice){
    vector<Constituency> list = loadAllConstituencies();
      for (const auto &c : list)
      {
         if (c.getConstituencyID() == ConstituencyIDPA && c.getCityID() == cityChoice)
            return true;
      }
      return false;
}

// Admin: Add constituency
void addConstituency(const Constituency &newConst)
{
    vector<Constituency> list = loadAllConstituencies();

    // Validation: ID positive, unique; Name non-empty, unique; CityID positive
    if (!isValidConstituencyID(newConst.getConstituencyID()))
    {
       ShowMessage(screen,"Error: Constituency ID must be positive.","error");
       return;
    }
    if (!isValidConstituencyName(newConst.getConstituencyName()))
    {
       ShowMessage(screen,"Error: Constituency name is invalid or too long.","error");
       return;
    }
    if (constituencyNameExists(list, newConst.getConstituencyName()))
    {
       ShowMessage(screen,"Error: Constituency name already exists.","error");
       return;
    }
    if (!cityExists(newConst.getCityID()))
    {
       ShowMessage(screen,"Error: City ID does not exist.","error");
       return;
    }
    if (!electionExists(newConst.getElectionID()))
    {
       ShowMessage(screen,"Error: Election ID does not exist.","error");
       return;
    }

    list.push_back(newConst);
                             ShowProgressBar(screen, "Adding Constituency...");
    saveAllConstituencies(list);
    ShowMessage(screen,"Constituency added.","success");
}

// Admin: Edit constituency name
void editConstituency(int id, const string &newName, int CityID, int ElectionID)
{
    vector<Constituency> list = loadAllConstituencies();
    bool found = false;

    if (!isValidConstituencyID(id))
    {
       ShowMessage(screen,"Error: Invalid constituency ID.","error");
       return;
    }
    if (!isValidConstituencyName(newName))
    {
       ShowMessage(screen,"Error: New name is invalid or too long.","error");
       return;
    }
    if (constituencyNameExists(list, newName))
    {
       ShowMessage(screen,"Error: Constituency name already exists.","error");
       return;
    }
    if (!cityExists(CityID))
    {
       ShowMessage(screen,"Error: City ID does not exist.","error");
       return;
    }
    if (!electionExists(ElectionID))
    {
       ShowMessage(screen,"Error: Election ID does not exist.","error");
       return;
    }

    for (auto &c : list)
    {
       if (c.getConstituencyID() == id)
       {
          c.setConstituencyName(newName);
          c.setCityID(CityID);
          c.setElectionID(ElectionID);
          found = true;
          break;
       }
    }
    if (!found)
    {
       ShowMessage(screen,"Error: Constituency ID not found.","error");
       return;
    }
                               ShowProgressBar(screen, "Editing Constituency...");
    saveAllConstituencies(list);
    ShowMessage(screen,"Constituency updated.","success");
}

// Admin: Delete constituency by ID
void deleteConstituency(int id)
{
    if (!isValidConstituencyID(id))
    {
       ShowMessage(screen,"Error: Invalid constituency ID.","error");
       return;
    }
    vector<Constituency> list = loadAllConstituencies();
    size_t before = list.size();
    auto it = remove_if(list.begin(), list.end(), [id](const Constituency &c)
                    { return c.getConstituencyID() == id; });
    list.erase(it, list.end());
    if (list.size() == before)
    {
       ShowMessage(screen,"Error: Constituency ID not found.","error");
       return;
    }
   ShowProgressBar(screen, "Deleting Constituency...");
    saveAllConstituencies(list);
    ShowMessage(screen,"Constituency deleted.","success");
}

// Admin/User: View all constituencies
void listAllConstituencies()
{
    vector<Constituency> list = loadAllConstituencies();
    if (list.empty())
    {
       ShowMessage(screen,"No constituencies found.","info");
       return;
    }
    
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
    ShowTableFTXUI(screen, "All Constituencies", headers, data);
}

// Admin/User: List by city
void listConstituenciesByCity(int cityID)
{
    if (!isValidConstituencyID(cityID))
    {
       ShowMessage(screen,"Error: Invalid city ID.","error");
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
       ShowMessage(screen,"No constituencies found for this city.","info");
       return;
    }
    ShowTableFTXUI(screen, "Constituencies in City " + to_string(cityID), headers, data);
}

bool constituencyExists(int id) {
    if (!isValidConstituencyID(id)) return false;
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
       if (c.getConstituencyID() == id) return true;
    }
    return false;
}

string getConstituencyTypeByID(int id) {
    if (!isValidConstituencyID(id)) return "";
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
       if (c.getConstituencyID() == id) {
          if (c.getConstituencyName().size() >= 2)
             return c.getConstituencyName().substr(0, 2);
          else
             return "";
       }
    }
    return "";
}

int getElectionIDByConstituencyID(int id) {
    if (!isValidConstituencyID(id)) return -1;
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
       if (c.getConstituencyID() == id) {
          return c.getElectionID();
       }
    }
    return -1; // Not found
}

void manageConstituencies() {
          ShowSpinner(screen, "Loading Constituencies...");

    vector<Constituency> list = loadAllConstituencies();
    while (true) {
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

          vector<InputField> form = {
             {"Election ID", &ElectionID_str, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Add Constituency", form);
          if (!success) {
             ShowMessage(screen, "Constituency creation cancelled.","error");
             continue;
          }
          int ElectionID = 0;
          try {
             ElectionID = stoi(ElectionID_str);
          } catch (...) {
             ShowMessage(screen, "Invalid Election ID format.","error");
             continue;
          }
          type = getElectionTypeByID(ElectionID);
          if (type.empty()) {
             ShowMessage(screen, "Invalid Election Type.","error");
             continue;
          }
          vector<InputField> form2 = {
             {"Constituency Name (" + type + "- )", &name, InputField::TEXT}
          };
          bool success2 = ShowForm(screen, "Add Constituency", form2);
          if (!success2) {
             ShowMessage(screen,"Constituency creation cancelled.","error");
             continue;
          }
          fullName = type + "-" + name;
          if (!electionExists(ElectionID)) {
             ShowMessage(screen,"Invalid Election ID.","error");
             continue;
          }
          if (!isValidConstituencyName(fullName)) {
             ShowMessage(screen,"Invalid Constituency Name.","error");
             continue;
          }
          if (constituencyNameExists(list, fullName)) {
             ShowMessage(screen,"Constituency Name already exists.","error");
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
             ShowMessage(screen,"City selection cancelled.","error");
             continue;
          }
          int cityID = 0;
          try {
             cityID = stoi(cityID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid City ID format.","error");
             continue;
          }
          if (!cityExists(cityID)) {
             ShowMessage(screen,"Invalid City ID.","error");
             continue;
          }
          Constituency c(getNextID("ConstituencyID"), fullName, cityID, ElectionID);
          addConstituency(c);

       } 
       else if (choice == 1) {
          listAllConstituencies();
       } else if (choice == 2) {
          string id_str, cityID_str, ElectionID_str, provinceName_str, name, type, fullName;
          listAllConstituencies();

          vector<InputField> form = {
             {"Constituency ID", &id_str, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Edit Constituency", form);
          if (!success) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!isValidConstituencyID(id)) {
             ShowMessage(screen,"Invalid Constituency ID.","error");
             continue;
          }
          if (!constituencyExists(id)) {
             ShowMessage(screen,"Constituency ID doesn't exist.","error");
             continue;
          }
          listAllElections();
          vector<InputField> form2 = {
             {"Election ID", &ElectionID_str, InputField::TEXT}
          };
          bool success2 = ShowForm(screen, "Edit Constituency", form2);
          if (!success2) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          int ElectionID = 0;
          try {
             ElectionID = stoi(ElectionID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Election ID format.","error");
             continue;
          }
          if (!electionExists(ElectionID)) {
             ShowMessage(screen,"Invalid Election ID.","error");
             continue;
          }
          type = getElectionTypeByID(ElectionID);
          if (type.empty()) {
             ShowMessage(screen,"Invalid Election Type.","error");
             continue;
          }
          vector<InputField> form3 = {
             {"Constituency Name (" + type + "- )", &name, InputField::TEXT}
          };
          bool success3 = ShowForm(screen, "Edit Constituency", form3);
          if (!success3) {
             ShowMessage(screen,"Edit cancelled.","error");
             continue;
          }
          fullName = type + "-" + name;
          if (!isValidConstituencyName(fullName)) {
             ShowMessage(screen,"Invalid Constituency Name.","error");
             continue;
          }
          if (constituencyNameExists(list, fullName)) {
             ShowMessage(screen,"Constituency Name already exists.","error");
             continue;
          }
          vector<InputField> form4 = {
             {"Select Province", &provinceName_str, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
          };
          bool success4 = ShowForm(screen, "Edit Constituency", form4);
          if (!success4) {
             ShowMessage(screen,"City selection cancelled.","error");
             continue;
          }
          listCitiesByProvince(provinceName_str);
          vector<InputField> form5 = {
             {"City ID", &cityID_str, InputField::TEXT}
          };
          bool success5 = ShowForm(screen, "Edit Constituency", form5);
          if (!success5) {
             ShowMessage(screen,"City selection cancelled.","error");
             continue;
          }
          int cityID = 0;
          try {
             cityID = stoi(cityID_str);
          } catch (...) {
             ShowMessage(screen,"Invalid City ID format.","error");
             continue;
          }
          if (!cityExists(cityID)) {
             ShowMessage(screen,"Invalid City ID.","error");
             continue;
          }
          editConstituency(id, fullName, cityID, ElectionID);
       } 
       else if (choice == 3) {
          string id_str;
          listAllConstituencies();

          vector<InputField> form = {
             {"Constituency ID", &id_str, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Delete Constituency", form);
          if (!success) {
             ShowMessage(screen,"Delete cancelled.","error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen,"Invalid Constituency ID format.","error");
             continue;
          }
          if (!isValidConstituencyID(id)) {
             ShowMessage(screen,"Invalid Constituency ID.","error");
             continue;
          }
          if (!constituencyExists(id)) {
             ShowMessage(screen,"Constituency ID not found.","error");
             continue;
          }
          deleteConstituency(id);
       } else if (choice == 4) {
          break;
       } else {
          ShowMessage(screen,"Invalid option.","error");
       }
    }
}