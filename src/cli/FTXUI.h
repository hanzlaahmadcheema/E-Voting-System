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
void ShowSpinner(ScreenInteractive& screen, const string& message);
void ShowProgressBar(ScreenInteractive& screen, const string& label);
void ShowMessage(ScreenInteractive& screen, const string& msg, const string& type);

#endif