#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
using namespace ftxui;
using namespace std;

extern int ShowMenu(ScreenInteractive& screen, 
             const std::string& heading, 
             const std::vector<std::string>& options);
