#include "PollingStation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
// PollingStation
PollingStation::PollingStation() : PollingStationID(0), PollingStationName(""), PollingStationAddress(""), ConstituencyID(0) {}
PollingStation::PollingStation(int PollingStationID, const string &PollingStationName, const string &PollingStationAddress, int ConstituencyID)
{
    this->PollingStationID = PollingStationID;
    this->PollingStationName = PollingStationName;
    this->PollingStationAddress = PollingStationAddress;
    this->ConstituencyID = ConstituencyID;
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
void PollingStation::setConstituencyID(int ConstituencyID)
{
    this->ConstituencyID = ConstituencyID;
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
int PollingStation::getConstituencyID() const
{
    return ConstituencyID;
}
void PollingStation::displayPollingStationInfo() const
{
    cout << "Polling Station ID: " << PollingStationID << "\n"
         << "Name: " << PollingStationName << "\n"
         << "Address: " << PollingStationAddress << "\n"
         << "Constituency ID: " << ConstituencyID << endl;
}

json PollingStation::toJSON() const
{
    return json{
        {"PollingStationID", PollingStationID},
        {"PollingStationName", PollingStationName},
        {"PollingStationAddress", PollingStationAddress},
        {"ConstituencyID", ConstituencyID}};
}

PollingStation PollingStation::fromJSON(const json &j)
{
    return PollingStation(
        j.at("PollingStationID").get<int>(),
        j.at("PollingStationName").get<std::string>(),
        j.at("PollingStationAddress").get<std::string>(),
        j.at("ConstituencyID").get<int>());
}

const string STATION_FILE = "data/polling_stations.json";

// Helper: Check if string is empty or whitespace
bool isBlank(const string &str)
{
    return str.find_first_not_of(" \t\n\r") == string::npos;
}

// Helper: Check if station ID is unique
bool isUniqueStationID(int id, const vector<PollingStation> &list)
{
    for (const auto &s : list)
    {
        if (s.getPollingStationID() == id)
            return false;
    }
    return true;
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
    if (!isUniqueStationID(s.getPollingStationID(), list))
    {
        cout << "Polling Station ID already exists.\n";
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
    if (s.getConstituencyID() <= 0)
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
        if (s.getConstituencyID() == constID)
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
            int id, ConstituencyID;
            string name, address;
            cout << "Enter Station ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Station Name: "; getline(cin, name);
            cout << "Enter Address: "; getline(cin, address);
            cout << "Enter Constituency ID: "; cin >> ConstituencyID;
            PollingStation ps(id, name, address, ConstituencyID);
            addPollingStation(ps);
        } else if (choice == 2) {
            listAllStations();
        } else if (choice == 3) {
            int id;
            string name, address;
            cout << "Enter Station ID: "; cin >> id;
            cin.ignore();
            cout << "Enter New Name: "; getline(cin, name);
            cout << "Enter New Address: "; getline(cin, address);
            editPollingStation(id, name, address);
        } else if (choice == 4) {
            int id;
            cout << "Enter Station ID to delete: ";
            cin >> id;
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
//     PollingStation ps(1, "Main Street Station", "123 Main St", 101);
//     addPollingStation(ps);
//     listStationsByConstituency(101);
//     editPollingStation(1, "Updated Station", "456 Updated St");
//     listStationsByConstituency(101);
//     deletePollingStation(1);
//     listStationsByConstituency(101);
//     return 0;
// }