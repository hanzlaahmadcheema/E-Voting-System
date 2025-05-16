#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class PollingStation
{
public:
    PollingStation();
    PollingStation(int PollingStationID, const string &PollingStationName, const string &PollingStationAddress, int ConstituencyID);

    void setPollingStationID(int PollingStationID);
    void setPollingStationName(const string &PollingStationName);
    void setPollingStationAddress(const string &PollingStationAddress);
    void setConstituencyID(int ConstituencyID);
    int getPollingStationID() const;
    string getPollingStationName() const;
    string getPollingStationAddress() const;
    int getConstituencyID() const;
    void displayPollingStationInfo() const;

    json toJSON() const;
    static PollingStation fromJSON(const json &j);

    static const string POLLINGSTATION_FILE;
    static vector<PollingStation> loadAllPollingStations();
    static void saveAllPollingStations(const vector<PollingStation> &pollingStations);
    static void addPollingStation(const PollingStation &newPollingStation);
    static void deletePollingStationByID(int pollingStationID);
    static void listAllPollingStations();
    static void listPollingStationsByConstituency(int constituencyID);

private:
    int PollingStationID;
    string PollingStationName;
    string PollingStationAddress;
    int ConstituencyID;
};

#endif