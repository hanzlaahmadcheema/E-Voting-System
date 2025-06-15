#include <custom/config.h>

extern void adminPanel();
extern void userPanel();
extern int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options);
extern void ShowMessage(ScreenInteractive& screen, const string& message, const string& type);

ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

void MainMenu() {
    vector<string> main_menu = {
        "Admin Panel",
        "User Panel",
        "Exit"
    };

    int choice = -1;
    try {
        choice = ShowMenu(screen, "Main Menu", main_menu);
        if (choice < 0 || choice >= static_cast<int>(main_menu.size())) {
            ShowMessage(screen, "Invalid menu selection.", "error");
            return;
        }
    } catch (const std::exception& ex) {
        ShowMessage(screen, string("Error: ") + ex.what(), "error");
        return;
    } catch (...) {
        ShowMessage(screen, "Unknown error occurred.", "error");
        return;
    }

    try {
        switch (choice) {
            case 0: 
                adminPanel(); 
                break;
            case 1: 
                userPanel(); 
                break;
            default: 
                ShowMessage(screen, "Exiting...", "info"); 
                break;
        }
    } catch (const std::exception& ex) {
        ShowMessage(screen, string("Panel error: ") + ex.what(), "error");
    } catch (...) {
        ShowMessage(screen, "Unknown error in panel.", "error");
    }
}