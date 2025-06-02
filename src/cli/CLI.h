#ifndef CLI_H
#define CLI_H

#include <string>

void printDivider(const std::string& title);
void printSuccess(const std::string& msg);
void printError(const std::string& msg);
void printWarning(const std::string& msg);
void animateText(const std::string& text, int delay = 50);
void showProgressBar(int durationInMs);
void clearScreen();
int getTerminalWidth();

#endif