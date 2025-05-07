#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>

class Constituency {
public:
Constituency();
    Constituency(int id, const std::string& name, int cityID);
    void setConstituencyID(int id);
    void setName(const std::string& name);
    void setCityID(int cityID);
    int getConstituencyID() const;
    std::string getName() const;
    int getCityID() const;
    void displayConstituencyInfo() const;

private:
    int constituencyID;
    std::string name;
    int cityID;
};

#endif