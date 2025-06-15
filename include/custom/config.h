#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <functional>
#include <unordered_map>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <set>
#include <regex>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <json.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <cli/FTXUI.h>
#include <core/AdminCLI.h>
#include <core/Universal.h>
#include <core/UserCLI.h>
#include <core/VotingSystem.h>
#include <models/Candidate.h>
#include <models/City.h>
#include <models/Constituency.h>
#include <models/Election.h>
#include <models/Party.h>
#include <models/PollingStation.h>
#include <models/Result.h>
#include <models/Vote.h>
#include <models/Voter.h>


using namespace std;
using namespace ftxui;
using json = nlohmann::json;

void ShowSpinner(ScreenInteractive& screen, const string& message);
void ShowProgressBar(ScreenInteractive& screen, const string& label);
void ShowMessage(ScreenInteractive& screen, const string& msg, const string& type);

extern ftxui::ScreenInteractive screen;