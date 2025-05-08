#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>
using namespace std;

class PollingStation {
public:
PollingStation();
    PollingStation(int id, const string& name, const string& address, int constituencyID);
    
    void setPollingStationID(int id);
    void setName(const string& name);
    void setAddress(const string& address);
    void setConstituencyID(int constituencyID);
    int getPollingStationID() const;
    string getName() const;
    string getAddress() const;
    int getConstituencyID() const;
    void displayPollingStationInfo() const;

private:
    int pollingStationID;
    string name;
    string address;
    int constituencyID;
};

#endif