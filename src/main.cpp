#include <custom/config.h>


// External logic modules
extern void adminPanel();
extern void userPanel();
extern int ShowMenu(ScreenInteractive& screen, 
             const string& heading, 
             const vector<string>& options);

ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

int main() {
    

    vector<string> main_menu = {
        "Admin Panel",
        "User Panel",
        "Exit"
    };

    int choice = ShowMenu(screen, "Main Menu", main_menu);

    switch (choice) {
        case 0: adminPanel(); break;
        case 1: userPanel(); break;
        default: ShowMessage(screen,"Exiting...", "info");; break;
    }

    return 0;
}
