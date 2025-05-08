#include "Party.h"
#include <iostream>
#include <string>
using namespace std;
//Party
Party::Party() : partyID(0), name(""), symbol("") {}
Party::Party(int id, const string& name, const string& symbol) {
    partyID = id;
    this->name = name;
    this->symbol = symbol;
}
void Party::setPartyID(int id) {
    partyID = id;
}
void Party::setName(const string& name) {
    this->name = name;
}
void Party::setSymbol(const string& symbol) {
    this->symbol = symbol;
}
int Party::getPartyID() const {
    return partyID;
}
string Party::getName() const {
    return name;
}
string Party::getSymbol() const {
    return symbol;
}
void Party::displayPartyInfo() const {
    cout << "Party ID: " << partyID << "\n"
              << "Name: " << name << "\n"
              << "Symbol: " << symbol << endl;
}