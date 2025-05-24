#include "Constituency.h"
#include "City.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern int getNextID(const string &key);
extern bool cityExists(int id);
extern bool electionExists(int id);
extern void listAllCities();
extern void listAllElections();
extern void listCitiesByProvince(const string &province);
extern string getElectionTypeByID(int id);
extern string toLower(const string& str);

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
void Constituency::displayConstituencyInfo() const
{
    cout << "Constituency ID: " << ConstituencyID << "\n"
         << "Name: " << ConstituencyName << "\n"
         << "City ID: " << CityID << "\n"
         << "Election ID: " << ElectionID << endl;
}
// Add any other methods or member functions as needed
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
    return Constituency(
        j.at("ConstituencyID").get<int>(),
        j.at("ConstituencyName").get<std::string>(),
        j.at("CityID").get<int>(),
        j.at("ElectionID").get<int>());
}

const string CONSTITUENCY_FILE = "data/constituencies.json";

bool constituencyNameExists(const vector<Constituency> &list, const string &name)
{
    for (const auto &p : list)
    {
        if (toLower(p.getConstituencyName()) == toLower(name))
            return true;
    }
    return false;
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
                list.push_back(Constituency::fromJSON(obj));
            }
        }
        catch (...)
        {
            cerr << "Error: Invalid JSON format in constituency file.\n";
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
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }
    json j;
    for (const auto &c : list)
    {
        j.push_back(c.toJSON());
    }
    file << j.dump(4);
}

// Admin: Add constituency
void addConstituency(const Constituency &newConst)
{
    vector<Constituency> list = loadAllConstituencies();

    // Validation: ID positive, unique; Name non-empty, unique; CityID positive
    if (newConst.getConstituencyID() <= 0)
    {
        cout << "Error: Constituency ID must be positive.\n";
        return;
    }
    if (newConst.getConstituencyName().empty())
    {
        cout << "Error: Constituency name cannot be empty.\n";
        return;
    }
    if (constituencyNameExists(list, newConst.getConstituencyName()))
    {
        cout << "Error: Constituency name already exists.\n";
        return;
    }
    if (newConst.getCityID() <= 0)
    {
        cout << "Error: City ID must be positive.\n";
        return;
    }

    list.push_back(newConst);
    saveAllConstituencies(list);
    cout << "Constituency added.\n";
}

// Admin: Edit constituency name
void editConstituency(int id, const string &newName, int CityID, int ElectionID)
{
    vector<Constituency> list = loadAllConstituencies();
    bool found = false;

    if (id <= 0)
    {
        cout << "Error: Invalid constituency ID.\n";
        return;
    }
    if (newName.empty())
    {
        cout << "Error: New name cannot be empty.\n";
        return;
    }
    if (constituencyNameExists(list, newName))
    {
        cout << "Error: Constituency name already exists.\n";
        return;
    }

    for (auto &c : list)
    {
        if (c.getConstituencyID() == id)
        {
            c.setConstituencyName(newName);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Error: Constituency ID not found.\n";
        return;
    }
    saveAllConstituencies(list);
    cout << "Constituency updated.\n";
}

// Admin: Delete constituency by ID
void deleteConstituency(int id)
{
    if (id <= 0)
    {
        cout << "Error: Invalid constituency ID.\n";
        return;
    }
    vector<Constituency> list = loadAllConstituencies();
    size_t before = list.size();
    auto it = remove_if(list.begin(), list.end(), [id](const Constituency &c)
                        { return c.getConstituencyID() == id; });
    list.erase(it, list.end());
    if (list.size() == before)
    {
        cout << "Error: Constituency ID not found.\n";
        return;
    }
    saveAllConstituencies(list);
    cout << "Constituency deleted.\n";
}

// Admin/User: View all constituencies
void listAllConstituencies()
{
    vector<Constituency> list = loadAllConstituencies();
    if (list.empty())
    {
        cout << "No constituencies found.\n";
        return;
    }
    for (const auto &c : list)
    {
        cout << c.getConstituencyID() << " - " << c.getConstituencyName() << endl;
    }
}

// Admin/User: List by city
void listConstituenciesByCity(int cityID)
{
    if (cityID <= 0)
    {
        cout << "Error: Invalid city ID.\n";
        return;
    }
    vector<Constituency> list = loadAllConstituencies();
    bool found = false;
    for (const auto &c : list)
    {
        if (c.getCityID() == cityID)
        {
            cout << c.getConstituencyID() << " - " << c.getConstituencyName() << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No constituencies found for this city.\n";
    }
}

bool constituencyExists(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) return true;
    }
    return false;
}

string getConstituencyTypeByID(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) {
                    //the first 2 digits are type
            string type = c.getConstituencyName().substr(0, 2);
            return type;
        }


    }
    return "";
}

int getElectionIDByConstituencyID(int id) {
    vector<Constituency> list = loadAllConstituencies();
    for (const auto& c : list) {
        if (c.getConstituencyID() == id) {
            return c.getElectionID();
        }
    }
    return -1; // Not found
}

void manageConstituencies() {
    int choice;
    vector<Constituency> list = loadAllConstituencies();
    while (true) {
        cout << "\n Constituency Management\n";
        cout << "1. Add Constituency\n";
        cout << "2. View All Constituencies\n";
        cout << "3. Edit Constituency\n";
        cout << "4. Delete Constituency\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int cityID, ElectionID;
            string name, type, fullName;
            listAllElections();
            cout << "Enter Election ID: ";
            cin >> ElectionID;
            if (!electionExists(ElectionID)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            type = getElectionTypeByID(ElectionID);
            cout << "Enter Constituency Name: " << type << "-";
            cin.ignore();
            getline(cin, name);
            fullName = type + "-" + name;
            if (!isValidConstituencyName(fullName)) {
                cout << "Invalid Constituency Name.\n";
                continue;
            }
            if (constituencyNameExists(list, fullName)) {
                cout << "Constituency Name already exists.\n";
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
            cout << "Enter City ID: ";
            cin >> cityID;
            if (!cityExists(cityID)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            Constituency c(getNextID("ConstituencyID"), fullName, cityID, ElectionID);
            addConstituency(c);
        } else if (choice == 2) {
            listAllConstituencies();
        } else if (choice == 3) {
            int id, cityID, ElectionID;
            string name, type, fullName;
            cout << "List of Constituencies:\n";
            listAllConstituencies();
            cout << "Enter Constituency ID: ";
            cin >> id;
            if (!isValidConstituencyID(id)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            if (!constituencyExists(id)) {
                cout << "Constituency ID doesn't exists.\n";
                continue;
            }
            cin.ignore();
            listAllElections();
            cout << "Enter Election ID: ";
            cin >> ElectionID;
            if (!electionExists(ElectionID)) {
                cout << "Invalid Election ID.\n";
                continue;
            }
            type = getElectionTypeByID(ElectionID);
            cout << "Enter new name: " << type << "-";
            getline(cin, name);

            fullName = type + "-" + name;
            if (!isValidConstituencyName(fullName)) {
                cout << "Invalid Constituency Name.\n";
                continue;
            }
            if (constituencyNameExists(list, fullName)) {
                cout << "Constituency Name already exists.\n";
                continue;
            }
            listAllCities();
            cout << "Enter City ID: ";
            cin >> cityID;
            if (!cityExists(cityID)) {
                cout << "Invalid City ID.\n";
                continue;
            }
            editConstituency(id, fullName, cityID, ElectionID);
        } else if (choice == 4) {
            int id;
            listAllConstituencies();
            cout << "Enter Constituency ID to delete: ";
            cin >> id;
            if (!isValidConstituencyID(id)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            if (!constituencyExists(id)) {
                cout << "Constituency ID not found.\n";
                continue;
            }
            deleteConstituency(id);
        } else if (choice == 0) {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
}

// int main()
// {
//     // Example usage
//     Constituency c1(getNextID("ConstituencyID"), "Downtown", 101);
//     addConstituency(c1);
//     listConstituenciesByCity(101);
//     editConstituency(c1.getConstituencyID(), "Uptown");
//     listConstituenciesByCity(101);
//     deleteConstituency(c1.getConstituencyID());
//     listConstituenciesByCity(101);
//     return 0;
// }