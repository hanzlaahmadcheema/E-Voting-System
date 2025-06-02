#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;
using namespace std;

// External logic modules
extern void adminPanel();
extern void userPanel();
extern int ShowMenu(ScreenInteractive& screen, 
             const std::string& heading, 
             const std::vector<std::string>& options);


int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> main_menu = {
        "Admin Panel",
        "User Panel",
        "Exit"
    };

    int choice = ShowMenu(screen, "Main Menu", main_menu);

    switch (choice) {
        case 0: adminPanel(); break;
        case 1: userPanel(); break;
        default: std::cout << "Exiting...\n"; break;
    }

    return 0;
}
