#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>
using namespace std;

class PollingStation {
public:
PollingStation();
    PollingStation(int PollingStationID, const string& PollingStationName, const string& PollingStationAddress, int ConstituencyID);
    
    void setPollingStationID(int PollingStationID);
    void setPollingStationName(const string& PollingStationName);
    void setPollingStationAddress(const string& PollingStationAddress);
    void setConstituencyID(int ConstituencyID);
    int getPollingStationID() const;
    string getPollingStationName() const;
    string getPollingStationAddress() const;
    int getConstituencyID() const;
    void displayPollingStationInfo() const;

private:
    int PollingStationID;
    string PollingStationName;
    string PollingStationAddress;
    int ConstituencyID;
};

#endif