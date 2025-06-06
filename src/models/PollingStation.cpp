#include "PollingStation.h"
#include "../core/Universal.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;
using namespace ftxui;
extern int getNextID(const string& key);
extern bool constituencyExists(int id);
extern void listAllConstituencies();
extern void listCitiesByProvince(const string &province);
extern void listConstituenciesByCity(int cityID);
extern int ShowMenu(ScreenInteractive & screen, 
     const std::string& heading, 
     const std::vector<std::string>& options);

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
void PollingStation::displayPollingStationInfo() const
{
    cout << "Polling Station ID: " << PollingStationID << "\n"
         << "Name: " << PollingStationName << "\n"
         << "Address: " << PollingStationAddress << "\n"
         << "Constituency ID2: " << ConstituencyIDNA << "\n"
         << "Constituency ID1: " << ConstituencyIDPA << endl;
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
        j.at("PollingStationName").get<std::string>(),
        j.at("PollingStationAddress").get<std::string>(),
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
    if (s.getConstituencyIDNA() <= 0)
    {
        cout << "Invalid Constituency ID.\n";
        return;
    }
    if (s.getConstituencyIDPA() <= 0)
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
        if (s.getConstituencyIDNA() == constID)
        {
            cout << s.getPollingStationID() << " - " << s.getPollingStationName()
                 << " (" << s.getPollingStationAddress() << ")" << endl;
            found = true;
        }
                if (s.getConstituencyIDPA() == constID)
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

void listStationsByCity(int cityID)
{
    if (cityID <= 0)
    {
        cout << "Invalid City ID.\n";
        return;
    }
    vector<PollingStation> list = loadAllStations();
    bool found = false;
    for (const auto &s : list)
    {
        if (s.getCityID() == cityID)
        {
            cout << s.getPollingStationID() << " - " << s.getPollingStationName()
                 << " (" << s.getPollingStationAddress() << ")" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No polling stations found for this city.\n";
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
           auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> pollingStationMenu = {
        "Add Polling Station",
        "View All Polling Stations",
        "Edit Polling Station",
        "Delete Polling Station",
        "Back"
    };

    int choice = ShowMenu(screen, "Polling Station Management", pollingStationMenu);
        if (choice == 0) {
            int choice, cityChoice, ConstituencyIDNA, ConstituencyIDPA;
            string name, address;
            cin.ignore();

            cout << "Select Province: " << endl;
            cout << "1. Punjab\n" 
                    "2. KPK\n" 
                    "3. Sindh\n"
                    "4. Balochistan" << endl;
            cin >> choice;
            if (choice < 1 || choice > 4) {
                cout << "Invalid province choice.\n";
                continue;
            }
            cin.ignore();
            if (choice == 1) {
                listCitiesByProvince("Punjab");
            } else if (choice == 2) {
                listCitiesByProvince("KPK");
            } else if (choice == 3) {
                listCitiesByProvince("Sindh");
            } else if (choice == 4) {
                listCitiesByProvince("Balochistan");
            }
            cout << "Select City:";
            cin >> cityChoice;
            cin.ignore();
            listStationsByCity(cityChoice);
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
            listConstituenciesByCity(cityChoice);
            cout << "Enter Constituency ID for National Assembly: "; cin >> ConstituencyIDNA;
            if (!constituencyExists(ConstituencyIDNA)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            cout << "Enter Constituency ID for Provincial Assembly: "; cin >> ConstituencyIDPA;
            if (!constituencyExists(ConstituencyIDPA)) {
                cout << "Invalid Constituency ID.\n";
                continue;
            }
            PollingStation ps(getNextID("PollingStationID"), name, address, cityChoice, ConstituencyIDNA, ConstituencyIDPA);
            addPollingStation(ps);
        } else if (choice == 1) {
            listAllStations();
        } else if (choice == 2) {
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
        } else if (choice == 3) {
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
        } else if (choice == 4) {
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