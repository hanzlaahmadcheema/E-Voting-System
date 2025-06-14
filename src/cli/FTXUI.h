#ifndef FTXUI_H
#define FTXUI_H

#include <custom/config.h>

struct InputField {
    string label;
    string* value;              // Pointer to actual variable to update
    enum Type { TEXT, NUMBER, DROPDOWN, RADIO } type;
    vector<string> options;
};
bool ShowForm(ScreenInteractive& screen, const string& title, vector<InputField>& fields);

int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options);
void ShowTableFTXUI(const vector<string>& headers, const vector<vector<string>>& rows, const string& title);
void ShowSpinner(ftxui::ScreenInteractive& screen, const std::string& message);
void ShowProgressBar(ftxui::ScreenInteractive& screen, const std::string& label);
void ShowMessage(ScreenInteractive& screen, const std::string& msg, const std::string& type);

#endif