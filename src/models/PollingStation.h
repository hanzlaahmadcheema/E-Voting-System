#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>

class PollingStation {
public:
PollingStation();
    PollingStation(int id, const std::string& name, const std::string& address, int constituencyID);
    
    void setPollingStationID(int id);
    void setName(const std::string& name);
    void setAddress(const std::string& address);
    void setConstituencyID(int constituencyID);
    int getPollingStationID() const;
    std::string getName() const;
    std::string getAddress() const;
    int getConstituencyID() const;
    void displayPollingStationInfo() const;

private:
    int pollingStationID;
    std::string name;
    std::string address;
    int constituencyID;
};

#endif