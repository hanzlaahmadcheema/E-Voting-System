#include "Constituency.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Constituency
Constituency::Constituency() : ConstituencyID(0), ConstituencyName(""), CityID(0) {}
Constituency::Constituency(int ConstituencyID, const string &ConstituencyName, int CityID)
{
    this->ConstituencyID = ConstituencyID;
    this->ConstituencyName = ConstituencyName;
    this->CityID = CityID;
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
void Constituency::displayConstituencyInfo() const
{
    cout << "Constituency ID: " << ConstituencyID << "\n"
         << "Name: " << ConstituencyName << "\n"
         << "City ID: " << CityID << endl;
}
// Add any other methods or member functions as needed
json Constituency::toJSON() const
{
    return json{
        {"ConstituencyID", ConstituencyID},
        {"ConstituencyName", ConstituencyName},
        {"CityID", CityID}};
}
Constituency Constituency::fromJSON(const json &j)
{
    return Constituency(
        j.at("ConstituencyID").get<int>(),
        j.at("ConstituencyName").get<std::string>(),
        j.at("CityID").get<int>());
}

const string CONSTITUENCY_FILE = "data/constituencies.json";

// Helper: Check if constituency ID exists
bool constituencyIDExists(const vector<Constituency> &list, int id)
{
    return any_of(list.begin(), list.end(), [id](const Constituency &c)
                  { return c.getConstituencyID() == id; });
}

// Helper: Check if constituency name exists (case-insensitive)
bool constituencyNameExists(const vector<Constituency> &list, const string &name)
{
    return any_of(list.begin(), list.end(), [&name](const Constituency &c)
                  {
        string n1 = c.getConstituencyName(), n2 = name;
        transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
        transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
        return n1 == n2; });
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
    if (constituencyIDExists(list, newConst.getConstituencyID()))
    {
        cout << "Error: Constituency ID already exists.\n";
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
void editConstituency(int id, const string &newName)
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

void manageConstituencies() {
    int choice;
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
            int id, cityID;
            string name;
            cout << "Enter Constituency ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name (e.g., NA-123): ";
            getline(cin, name);
            cout << "Enter City ID: ";
            cin >> cityID;
            Constituency c(id, name, cityID);
            addConstituency(c);
        } else if (choice == 2) {
            listAllConstituencies();
        } else if (choice == 3) {
            int id;
            string name;
            cout << "Enter Constituency ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, name);
            editConstituency(id, name);
        } else if (choice == 4) {
            int id;
            cout << "Enter Constituency ID to delete: ";
            cin >> id;
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
//     Constituency c1(1, "Downtown", 101);
//     addConstituency(c1);
//     listConstituenciesByCity(101);
//     editConstituency(1, "Uptown");
//     listConstituenciesByCity(101);
//     deleteConstituency(1);
//     listConstituenciesByCity(101);
//     return 0;
// }