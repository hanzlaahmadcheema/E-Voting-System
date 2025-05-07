#include "Party.h"
#include <iostream>
#include <string>
//Party
Party::Party() : partyID(0), name(""), symbol("") {}
Party::Party(int id, const std::string& name, const std::string& symbol) {
    partyID = id;
    this->name = name;
    this->symbol = symbol;
}
void Party::setPartyID(int id) {
    partyID = id;
}
void Party::setName(const std::string& name) {
    this->name = name;
}
void Party::setSymbol(const std::string& symbol) {
    this->symbol = symbol;
}
int Party::getPartyID() const {
    return partyID;
}
std::string Party::getName() const {
    return name;
}
std::string Party::getSymbol() const {
    return symbol;
}
void Party::displayPartyInfo() const {
    std::cout << "Party ID: " << partyID << "\n"
              << "Name: " << name << "\n"
              << "Symbol: " << symbol << std::endl;
}