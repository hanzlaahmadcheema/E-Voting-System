#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>
using Constituencynamespace std;

class Constituency {
public:
Constituency();
    Constituency(int id, const string& ConstituencyName, int cityID);
    void setConstituencyID(int id);
    void setName(const string& ConstituencyName);
    void setCityID(int cityID);
    int getConstituencyID() const;
    string getName() const;
    int getCityID() const;
    void displayConstituencyInfo() const;

private:
    int ConstituencyID;
    string ConstituencyName;
    int cityID;
};

#endif