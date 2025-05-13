#include "Party.h"
#include <iostream>
#include <string>
using namespace std;
//Party
Party::Party() : PartyID(0), PartyName(""), PartySymbol("") {}
Party::Party(int PartyID, const string& PartyName, const string& PartySymbol) {
    this->PartyID = PartyID;
    this->PartyName = PartyName;
    this->PartySymbol = PartySymbol;
}
void Party::setPartyID(int PartyID) {
    this->PartyID = PartyID;
}
void Party::setPartyName(const string& PartyName) {
    this->PartyName = PartyName;
}
void Party::setSymbol(const string& PartySymbol) {
    this->PartySymbol = PartySymbol;
}
int Party::getPartyID() const {
    return PartyID;
}
string Party::getName() const {
    return PartyName;
}
string Party::getSymbol() const {
    return PartySymbol;
}
void Party::displayPartyInfo() const {
    cout << "Party ID: " << PartyID << "\n"
              << "Name: " << PartyName << "\n"
              << "Symbol: " << PartySymbol << endl;
}