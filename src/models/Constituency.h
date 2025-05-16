#ifndef CONSTITUENCY_H
#define CONSTITUENCY_H

#include <string>
#include "../../include/json.hpp"

using namespace std;
using json = nlohmann::json;

class Constituency
{
public:
    Constituency();
    Constituency(int ConstituencyID, const string &ConstituencyName, int CityID);
    void setConstituencyID(int ConstituencyID);
    void setConstituencyName(const string &ConstituencyName);
    void setCityID(int CityID);
    int getConstituencyID() const;
    string getConstituencyName() const;
    int getCityID() const;
    void displayConstituencyInfo() const;

    json toJSON() const;
    static Constituency fromJSON(const json &j);

    static const string CONSTITUENCY_FILE;
    static vector<Constituency> loadAllConstituencies();
    static void saveAllConstituencies(const vector<Constituency> &constituencies);
    static void addConstituency(const Constituency &newConstituency);
    static void editConstituency(const Constituency &newConstituency);
    static void deleteConstituency(int ConstituencyID);
    static void listConstituenciesByCity(int cityID);
    static bool constituencyExists(int id);
    static void manageConstituencies();

private:
    int ConstituencyID;
    string ConstituencyName;
    int CityID;
};

#endif