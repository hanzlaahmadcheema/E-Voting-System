#ifndef POLLINGSTATION_H
#define POLLINGSTATION_H

#include <string>

class PollingStation {
public:
PollingStation();
    PollingStation(int id, const std::string& name, const std::string& address, int constituencyID);
    // Add getters and setters here

private:
    int pollingStationID;
    std::string name;
    std::string address;
    int constituencyID;
};

#endif