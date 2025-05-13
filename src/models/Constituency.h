#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>
using namespace std;

class Constituency {
public:
Constituency();
    Constituency(int ConstituencyID, const string& ConstituencyName, int CityID);
    void setConstituencyID(int ConstituencyID);
    void setConstituencyName(const string& ConstituencyName);
    void setCityID(int CityID);
    int getConstituencyID() const;
    string getName() const;
    int getCityID() const;
    void displayConstituencyInfo() const;

private:
    int ConstituencyID;
    string ConstituencyName;
    int CityID;
};

#endif