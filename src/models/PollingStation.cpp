#include "PollingStation.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

extern int getNextID(const string& key);
extern bool constituencyExists(int id);
extern void listAllConstituencies();

// PollingStation
PollingStation::PollingStation() : PollingStationID(0), PollingStationName(""), PollingStationAddress(""), ConstituencyID1(0), ConstituencyID2(0) {}
PollingStation::PollingStation(int PollingStationID, const string &PollingStationName, const string &PollingStationAddress, int ConstituencyID1, int ConstituencyID2)
{
    this->PollingStationID = PollingStationID;
    this->PollingStationName = PollingStationName;
    this->PollingStationAddress = PollingStationAddress;
    this->ConstituencyID1 = ConstituencyID1;
    this->ConstituencyID2 = ConstituencyID2;
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
void PollingStation::setConstituencyID1(int ConstituencyID1)
{
    this->ConstituencyID1 = ConstituencyID1;
}
void PollingStation::setConstituencyID2(int ConstituencyID2)
{
    this->ConstituencyID2 = ConstituencyID2;
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
int PollingStation::getConstituencyID1() const
{
    return ConstituencyID1;
}
int PollingStation::getConstituencyID2() const
{
    return ConstituencyID2;
}
void PollingStation::displayPollingStationInfo() const
{
    cout << "Polling Station ID: " << PollingStationID << "\n"
         << "Name: " << PollingStationName << "\n"
         << "Address: " << PollingStationAddress << "\n"
         << "Constituency ID2: " << ConstituencyID1 << "\n"
         << "Constituency ID1: " << ConstituencyID2 << endl;
}

json PollingStation::toJSON() const
{
    return json{
        {"PollingStationID", PollingStationID},
        {"PollingStationName", PollingStationName},
        {"PollingStationAddress", PollingStationAddress},
        {"ConstituencyID1", ConstituencyID1},
        {"ConstituencyID2", ConstituencyID2}};
}

PollingStation PollingStation::fromJSON(const json &j)
{
    return PollingStation(
        j.at("PollingStationID").get<int>(),
        j.at("PollingStationName").get<std::string>(),
        j.at("PollingStationAddress").get<std::string>(),
        j.at("ConstituencyID1").get<int>(),
        j.at("ConstituencyID2").get<int>());
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
            cerr << "Error: Corrupted polling station data.\n";
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
        cerr << "Error: Cannot open file to save polling stations.\n";
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
        cout << "Invalid Polling Station ID.\n";
        return;
    }
    if (isBlank(s.getPollingStationName()))
    {
        cout << "Polling Station name cannot be empty.\n";
        return;
    }
    if (isBlank(s.getPollingStationAddress()))
    {
        cout << "Polling Station address cannot be empty.\n";
        return;
    }
    if (s.getConstituencyID1() <= 0)
    {
        cout << "Invalid Constituency ID.\n";
        return;
    }
    if (s.getConstituencyID2() <= 0)
    {
        cout << "Invalid Constituency ID.\n";
        return;
    }
    list.push_back(s);
    saveAllStations(list);
    cout << "Polling station added.\n";
}

// Admin: Edit station
void editPollingStation(int id, const string &newName, const string &newAddress)
{
    vector<PollingStation> list = loadAllStations();
    bool found = false;

    if (id <= 0)
    {
        cout << "Invalid Polling Station ID.\n";
        return;
    }
    if (isBlank(newName))
    {
        cout << "New name cannot be empty.\n";
        return;
    }
    if (isBlank(newAddress))
    {
        cout << "New address cannot be empty.\n";
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
        cout << "Polling station not found.\n";
        return;
    }
    saveAllStations(list);
    cout << "Polling station updated.\n";
}

// Admin: Delete station
void deletePollingStation(int id)
{
    if (id <= 0)
    {
        cout << "Invalid Polling Station ID.\n";
        return;
    }
    vector<PollingStation> list = loadAllStations();
    auto it = remove_if(list.begin(), list.end(), [id](const PollingStation &s)
                        { return s.getPollingStationID() == id; });
    if (it == list.end())
    {
        cout << "Polling station not found.\n";
        return;
    }
    list.erase(it, list.end());
    saveAllStations(list);
    cout << "Polling station deleted.\n";
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
        cout << "Invalid Constituency ID.\n";
        return;
    }
    vector<PollingStation> list = loadAllStations();
    bool found = false;
    for (const auto &s : list)
    {
        if (s.getConstituencyID1() == constID)
        {
            cout << s.getPollingStationID() << " - " << s.getPollingStationName()
                 << " (" << s.getPollingStationAddress() << ")" << endl;
            found = true;
        }
                if (s.getConstituencyID2() == constID)
        {
            cout << s.getPollingStationID() << " - " << s.getPollingStationName()
                 << " (" << s.getPollingStationAddress() << ")" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No polling stations found for this constituency.\n";
    }
}

void listAllStations()
{
    vector<PollingStation> list = loadAllStations();
    if (list.empty())
    {
        cout << "ℹNo polling stations found.\n";
        return;
    }
    for (const auto &s : list)
    {
        cout << s.getPollingStationID() << " - " << s.getPollingStationName()
             << " (" << s.getPollingStationAddress() << ")" << endl;
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
        cout << "\n Polling Station Management\n";
        cout << "1. Add Polling Station\n";
        cout << "2. View All Polling Stations\n";
        cout << "3. Edit Polling Station\n";
        cout << "4. Delete Polling Station\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int ConstituencyID1, ConstituencyID2;
            string name, address;
            cin.ignore();
            cout << "Enter Station Name: "; getline(cin, name);
            if (!isValidPollingStationName(name)) {
                cout << "Invalid Polling Station Name.\n";
                continue;
            }
            cout << "Enter Address: "; getline(cin, address);
            if (!isValidPollingStationAddress(address)) {
                cout << "Invalid Polling Station Address.\n";
                continue;
            }
            listAllConstituencies();
            cout << "Enter Constituency ID: "; cin >> ConstituencyID1;
            if (!constituencyExists(ConstituencyID1)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            cout << "Enter Constituency ID: "; cin >> ConstituencyID2;
            if (!constituencyExists(ConstituencyID2)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            PollingStation ps(getNextID("PollingStationID"), name, address, ConstituencyID1, ConstituencyID2);
            addPollingStation(ps);
        } else if (choice == 2) {
            listAllStations();
        } else if (choice == 3) {
            int id;
            string name, address;
            cout << "List of Polling Stations:\n";
            listAllStations();
            cout << "Enter Station ID to edit: "; cin >> id;
            if (!isValidPollingStationID(id)) {
                cout << "Invalid Polling Station ID.\n";
                continue;
            }
            if (!pollingStationExists(id)) {
                cout << "Polling Station ID not found.\n";
                continue;
            }
            cin.ignore();
            cout << "Enter New Name: "; getline(cin, name);
            if (!isValidPollingStationName(name)) {
                cout << "Invalid Polling Station Name.\n";
                continue;
            }
            cout << "Enter New Address: "; getline(cin, address);
            if (!isValidPollingStationAddress(address)) {
                cout << "Invalid Polling Station Address.\n";
                continue;
            }
            editPollingStation(id, name, address);
        } else if (choice == 4) {
            int id;
            listAllStations();
            cout << "Enter Station ID to delete: ";
            cin >> id;
            if (!isValidPollingStationID(id)) {
                cout << "Invalid Polling Station ID.\n";
                continue;
            }
            if (!pollingStationExists(id)) {
                cout << "Polling Station ID not found.\n";
                continue;
            }
            deletePollingStation(id);
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
//     PollingStation ps(getNextID("PollingStationID"), "Main Street Station", "123 Main St", 101);
//     addPollingStation(ps);
//     listStationsByConstituency(101);
//     editPollingStation(ps.getPollingStationID(), "Updated Station", "456 Updated St");
//     listStationsByConstituency(101);
//     deletePollingStation(ps.getPollingStationID());
//     listStationsByConstituency(101);
//     return 0;
// }