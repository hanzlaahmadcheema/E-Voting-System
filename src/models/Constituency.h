#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>
using namespace std;

class Constituency {
public:
Constituency();
    Constituency(int id, const string& name, int cityID);
    void setConstituencyID(int id);
    void setName(const string& name);
    void setCityID(int cityID);
    int getConstituencyID() const;
    string getName() const;
    int getCityID() const;
    void displayConstituencyInfo() const;

private:
    int constituencyID;
    string name;
    int cityID;
};

#endif