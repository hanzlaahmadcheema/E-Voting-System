#include <custom/config.h>
#include <stdexcept>
#include <limits>

extern int getNextID(const string &key);
extern int ShowMenu(ScreenInteractive & screen, 
    const string& heading, 
    const vector<string>& options);
void ShowTableFTXUI(const string& heading, 
                const vector<string>& headers, 
                const vector<vector<string>>& rows);
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

;

// City
City::City() : CityID(0), CityName(""), ProvinceName("") {}
City::City(int CityID, const string &CityName, const string &ProvinceName)
{
    this->CityID = CityID;
    this->CityName = CityName;
    this->ProvinceName = ProvinceName;
}
void City::setCityID(int CityID)
{
    this->CityID = CityID;
}
void City::setCityName(const string &CityName)
{
    this->CityName = CityName;
}
void City::setProvinceName(const string &ProvinceName)
{
    this->ProvinceName = ProvinceName;
}
int City::getCityID() const
{
    return CityID;
}
string City::getCityName() const
{
    return CityName;
}
string City::getProvinceName() const
{
    return ProvinceName;
}

json City::toJSON() const
{
    return json{
       {"CityID", CityID},
       {"CityName", CityName},
       {"ProvinceName", ProvinceName}};
}
City City::fromJSON(const json &j)
{
    return City(
       j.at("CityID").get<int>(),
       j.at("CityName").get<string>(),
       j.at("ProvinceName").get<string>());
}

const string CITY_FILE = "data/cities.json";

bool cityNameExists(const vector<City> &list, const string &name)
{
    for (const auto &c : list)
    {
       if (c.getCityName() == name)
          return true;
    }
    return false;
}

bool isValidCityName(const string &name)
{
    return !name.empty() && name.length() <= 50 && name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos;
}
bool isValidCityID(int id)
{
    return id > 0 && id < std::numeric_limits<int>::max();
}
bool isValidProvinceName(const string& name)
{
    static const vector<string> validProvinces = {"Punjab", "KPK", "Sindh", "Balochistan"};
    return std::find(validProvinces.begin(), validProvinces.end(), name) != validProvinces.end();
}

// Load all cities
vector<City> loadAllCities()
{
    vector<City> cities;
    ifstream file(CITY_FILE);
    if (file.is_open())
    {
       try
       {
          json j;
          file >> j;
          if (!j.is_array())
          {
             cerr << "Invalid cities data format.";
             return cities;
          }
          for (auto &obj : j)
          {
             try
             {
                cities.push_back(City::fromJSON(obj));
             }
             catch (const exception &e)
             {
                cerr << "Error parsing city: " << e.what() << endl;
             }
          }
       }
       catch (const exception &e)
       {
          cerr << "Error reading cities file: " << e.what() << endl;
       }
    }
    else
    {
       cerr << "Could not open cities file for reading.";
    }
    return cities;
}

// Save all cities
void saveAllCities(const vector<City> &cities)
{
    try {
       ofstream file(CITY_FILE);
       if (!file.is_open())
       {
          cerr << "Could not open cities file for writing.";
          return;
       }
       json j = json::array();
       for (const auto &c : cities)
       {
          j.push_back(c.toJSON());
       }
       file << j.dump(4);
    } catch (const exception& e) {
       cerr << "Error saving cities: " << e.what() << endl;
    }
}

// Admin: Add city
void addCity(const City &newCity)
{
    try {
       if (!isValidCityID(newCity.getCityID()))
       {
          ShowMessage(screen, "Invalid City ID. Must be positive.", "error");
          return;
       }
       if (!isValidCityName(newCity.getCityName()))
       {
          ShowMessage(screen, "Invalid or empty City name.", "error");
          return;
       }
       if (!isValidProvinceName(newCity.getProvinceName()))
       {
          ShowMessage(screen, "Invalid Province name.", "error");
          return;
       }
       vector<City> cities = loadAllCities();
       for (const auto &city : cities)
       {
          if (city.getCityID() == newCity.getCityID())
          {
             ShowMessage(screen, "City ID already exists.", "error");
             return;
          }
          if (city.getCityName() == newCity.getCityName())
          {
             ShowMessage(screen, "City name already exists.", "error");
             return;
          }
       }
       cities.push_back(newCity);
       saveAllCities(cities);
       ShowMessage(screen, "City added successfully.", "success");
    } catch (const exception& e) {
       ShowMessage(screen, string("Error adding city: ") + e.what(), "error");
    }
}

// Admin: Edit city by ID
void editCity(int cityID, const string &newName, const string &newProvinceName)
{
    try {
       if (!isValidCityID(cityID))
       {
          ShowMessage(screen, "Invalid City ID.", "error");
          return;
       }
       if (!isValidCityName(newName))
       {
          ShowMessage(screen, "Invalid or empty City name.", "error");
          return;
       }
       if (!isValidProvinceName(newProvinceName))
       {
          ShowMessage(screen, "Invalid Province name.", "error");
          return;
       }
       vector<City> cities = loadAllCities();
       bool found = false;
       for (const auto &city : cities)
       {
          if (city.getCityName() == newName && city.getCityID() != cityID)
          {
             ShowMessage(screen, "City name already exists.", "error");
             return;
          }
       }
       for (auto &city : cities)
       {
          if (city.getCityID() == cityID)
          {
             city.setCityName(newName);
             city.setProvinceName(newProvinceName);
             found = true;
             break;
          }
       }
       if (!found)
       {
          ShowMessage(screen, "City ID not found.", "error");
          return;
       }
       saveAllCities(cities);
       ShowMessage(screen, "City updated.", "success");
    } catch (const exception& e) {
       ShowMessage(screen, string("Error editing city: ") + e.what(), "error");
    }
}

// Admin: Delete city by ID
void deleteCityByID(int cityID)
{
    try {
       if (!isValidCityID(cityID))
       {
          ShowMessage(screen, "Invalid City ID.", "error");
          return;
       }
       vector<City> cities = loadAllCities();
       auto it = remove_if(cities.begin(), cities.end(), [cityID](const City &c)
                       { return c.getCityID() == cityID; });
       if (it == cities.end())
       {
          ShowMessage(screen, "City ID not found.", "error");
          return;
       }
       cities.erase(it, cities.end());
       saveAllCities(cities);
       ShowMessage(screen, "City deleted if existed.", "info");
    } catch (const exception& e) {
       ShowMessage(screen, string("Error deleting city: ") + e.what(), "error");
    }
}

// Admin/User: View all cities
void listCitiesByProvince(const string &province)
{
    try {
       if (!isValidProvinceName(province)) {
          ShowMessage(screen, "Invalid Province name.", "error");
          return;
       }
       vector<City> cities = loadAllCities();
       if (cities.empty())
       {
          ShowMessage(screen, "No cities found.", "info");
          return;
       }
       
       vector<string> headers = {"City ID", "City Name", "Province Name"};
       vector<vector<string>> data;
       for (const auto &c : cities)
       {
          if (c.getProvinceName() == province)
          {
             data.push_back({to_string(c.getCityID()), c.getCityName(), c.getProvinceName()});
          }
       }
       ShowTableFTXUI("Cities in " + province, headers, data);
    } catch (const exception& e) {
       ShowMessage(screen, string("Error listing cities: ") + e.what(), "error");
    }
}

void listAllCities()
{
    try {
       vector<City> cities = loadAllCities();
       if (cities.empty())
       {
          ShowMessage(screen, "No cities found.", "info");
          return;
       }
       
       vector<string> headers = {"City ID", "City Name", "Province Name"};
       vector<vector<string>> data;
       for (const auto &c : cities)
       {
          data.push_back({to_string(c.getCityID()), c.getCityName(), c.getProvinceName()});
       }
       ShowTableFTXUI("All Cities", headers, data);
    } catch (const exception& e) {
       ShowMessage(screen, string("Error listing cities: ") + e.what(), "error");
    }
}

bool cityExists(int id) {
    vector<City> list = loadAllCities();
    for (const auto& c : list) {
       if (c.getCityID() == id) return true;
    }
    return false;
}

void manageCities() {
    int choice;
    while (true) {
       vector<string> cityManagement = {
          "Add City",
          "View All Cities",
          "Edit City",
          "Delete City",
          "Back"
       };

       choice = ShowMenu(screen, "City Management", cityManagement);

       if (choice == 0) {
          string name, ProvinceName;
          vector<InputField> form = {
             {"City Name", &name, InputField::TEXT},
             {"Select Province", &ProvinceName, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
          };
          bool success = ShowForm(screen, "Add City", form);
          if (!success) {
             ShowMessage(screen, "City creation cancelled.", "error");
             continue;
          }
          if (!isValidCityName(name)) {
             ShowMessage(screen, "Invalid City Name.", "error");
             continue;
          }
          if (!isValidProvinceName(ProvinceName)) {
             ShowMessage(screen, "Invalid Province Name.", "error");
             continue;
          }
          int id = getNextID("CityID");
          addCity(City(id, name, ProvinceName));
       } else if (choice == 1) {
          listAllCities();
       } else if (choice == 2) {
          string id_str, name, ProvinceName;
          listAllCities();

          vector<InputField> form = {
             {"City ID", &id_str, InputField::NUMBER},
             {"New City Name", &name, InputField::TEXT}
          };
          bool success = ShowForm(screen, "Edit City", form);
          if (!success) {
             ShowMessage(screen, "Edit cancelled.", "error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen, "Invalid City ID format.", "error");
             continue;
          }
          if (!isValidCityID(id)) {
             ShowMessage(screen, "Invalid City ID.", "error");
             continue;
          }
          if (!cityExists(id)) {
             ShowMessage(screen, "City ID not found.", "info");
             continue;
          }
          if (!isValidCityName(name)) {
             ShowMessage(screen, "Invalid City Name.", "error");
             continue;
          }
          vector<InputField> form2 = {
             {"Province Name", &ProvinceName, InputField::DROPDOWN, {"Punjab", "KPK", "Sindh", "Balochistan"}}
          };
          bool success2 = ShowForm(screen, "Edit City", form2);
          if (!success2) {
             ShowMessage(screen, "Edit cancelled.", "error");
             continue;
          }
          if (!isValidProvinceName(ProvinceName)) {
             ShowMessage(screen, "Invalid Province Name.", "error");
             continue;
          }
          editCity(id, name, ProvinceName);
       } else if (choice == 3) {
          string id_str;
          listAllCities();

          vector<InputField> form3 = {
             {"City ID", &id_str, InputField::TEXT}
          };
          bool success3 = ShowForm(screen, "Delete City", form3);
          if (!success3) {
             ShowMessage(screen, "Delete cancelled.", "error");
             continue;
          }
          int id = 0;
          try {
             id = stoi(id_str);
          } catch (...) {
             ShowMessage(screen, "Invalid City ID format.", "error");
             continue;
          }
          if (!isValidCityID(id)) {
             ShowMessage(screen, "Invalid City ID.", "error");
             continue;
          }
          if (!cityExists(id)) {
             ShowMessage(screen, "City ID not found.", "info");
             continue;
          }
          deleteCityByID(id);
       } else if (choice == 4) {
          break;
       } else {
          ShowMessage(screen, "Invalid option.", "error");
       }
    }
}