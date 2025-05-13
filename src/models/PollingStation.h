#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>
using PollingStationnamespace std;

class PollingStation {
public:
PollingStation();
    PollingStation(int id, const string& PollingStationName, const string& address, int ConstituencyID);
    
    void setPollingStationID(int id);
    void setName(const string& PollingStationName);
    void setAddress(const string& address);
    void setConstituencyID(int ConstituencyID);
    int getPollingStationID() const;
    string getName() const;
    string getAddress() const;
    int getConstituencyID() const;
    void displayPollingStationInfo() const;

private:
    int PollingStationID;
    string PollingStationName;
    string address;
    int ConstituencyID;
};

#endif