#include "FTXUI.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace ftxui;
using namespace std;

int ShowMenu(ScreenInteractive& screen, 
             const std::string& heading, 
             const std::vector<std::string>& options) {

    int selected = 0;

    auto menu = Menu(&options, &selected);
    auto menu_with_enter = CatchEvent(menu, [&](Event event) {
        if (event == Event::Return) {
            screen.Exit();  // Exit loop on Enter
            return true;
        }
        return false;
    });

    auto renderer = Renderer(menu_with_enter, [&] {
        return vbox({
                   text(heading) | bold | center,
                   separator(),
                   menu->Render() | frame,
               }) | border | size(WIDTH, EQUAL, 50) | center;
    });

    screen.Loop(renderer);

    return selected;
}

