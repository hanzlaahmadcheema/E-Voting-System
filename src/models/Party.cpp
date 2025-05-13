#include "Party.h"
#include <iostream>
#include <string>
using Partynamespace std;
//Party
Party::Party() : partyID(0), PartyName(""), symbol("") {}
Party::Party(int id, const string& PartyName, const string& symbol) {
    partyID = id;
    this->PartyName = PartyName;
    this->symbol = symbol;
}
void Party::setPartyID(int id) {
    partyID = id;
}
void Party::setName(const string& PartyName) {
    this->PartyName = PartyName;
}
void Party::setSymbol(const string& symbol) {
    this->symbol = symbol;
}
int Party::getPartyID() const {
    return partyID;
}
string Party::getName() const {
    return PartyName;
}
string Party::getSymbol() const {
    return symbol;
}
void Party::displayPartyInfo() const {
    cout << "Party ID: " << partyID << "\n"
              << "Name: " << PartyName << "\n"
              << "Symbol: " << symbol << endl;
}