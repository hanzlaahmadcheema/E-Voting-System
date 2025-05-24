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
    PollingStation(int PollingStationID, const string &PollingStationName, const string &PollingStationAddress, int ConstituencyIDNA, int ConstituencyIDNA2);

    void setPollingStationID(int PollingStationID);
    void setPollingStationName(const string &PollingStationName);
    void setPollingStationAddress(const string &PollingStationAddress);
    void setConstituencyIDNA(int ConstituencyIDNA);
    void setConstituencyIDPA(int ConstituencyIDPA);
    int getPollingStationID() const;
    string getPollingStationName() const;
    string getPollingStationAddress() const;
    int getConstituencyIDNA() const;
    int getConstituencyIDPA() const;
    void displayPollingStationInfo() const;

    json toJSON() const;
    static PollingStation fromJSON(const json &j);

    static const string POLLINGSTATION_FILE;
    static vector<PollingStation> loadAllStations();
    static void saveAllStations(const vector<PollingStation> &pollingStations);
    static void addPollingStation(const PollingStation &newPollingStation);
    static void deletePollingStation(int pollingStationID);
    static void listAllStations();
    static void listStationsByConstituency(int ConstituencyID);
    static bool pollingStationExists(int id);
    static void managePollingStations();

private:
    int PollingStationID;
    string PollingStationName;
    string PollingStationAddress;
    int ConstituencyIDNA;
    int ConstituencyIDPA;
};

#endif