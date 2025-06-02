#include "CLI.h"
#include <iostream>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

const std::string leftPad = "                      ";

int getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return columns;
#else
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

//change bg color of terminal
// Standard ANSI escape codes do not support hex color codes directly.
// For 24-bit color (true color) terminals, you can use ANSI escape codes with RGB values.
// Example: "\033[48;2;R;G;Bm" for background color.
// This function supports both named colors and hex codes like "#RRGGBB".

void setTerminalBackgroundColor(const std::string& color) {
    std::string colorCode;
    if (color[0] == '#' && color.length() == 7) {
        // Parse hex color #RRGGBB
        int r = std::stoi(color.substr(1,2), nullptr, 16);
        int g = std::stoi(color.substr(3,2), nullptr, 16);
        int b = std::stoi(color.substr(5,2), nullptr, 16);
        colorCode = "\033[48;2;" + std::to_string(r) + ";" +
                    std::to_string(g) + ";" + std::to_string(b) + "m";
    } else if (color == "black") {
        colorCode = "\033[40m";
    } else if (color == "red") {
        colorCode = "\033[41m";
    } else if (color == "green") {
        colorCode = "\033[42m";
    } else if (color == "yellow") {
        colorCode = "\033[43m";
    } else if (color == "blue") {
        colorCode = "\033[44m";
    } else if (color == "magenta") {
        colorCode = "\033[45m";
    } else if (color == "cyan") {
        colorCode = "\033[46m";
    } else if (color == "light_gray") {
        colorCode = "\033[47m";
    } else {
        colorCode = RESET;
    }
    std::cout << colorCode;
}


void printDivider(const std::string& title) {
    int width = getTerminalWidth();
    std::string blockTitle = " " + title + " ";
    int blockWidth = blockTitle.size() + 4; // 2 for each side border
    int totalPadding = width - blockWidth;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    std::string topBottom = "+" + std::string(blockTitle.size() + 2, '=') + "+";
    std::string middle = "| " + blockTitle + " |";

    std::cout << std::string(leftPadding, ' ') << topBottom << std::endl;
    std::cout << std::string(leftPadding, ' ') << middle << std::endl;
    std::cout << std::string(leftPadding, ' ') << topBottom << std::endl;
}

void printSuccess(const std::string& msg) {
    std::cout << leftPad << GREEN << "[SUCCESS] " << msg << RESET << std::endl;
}

void printError(const std::string& msg) {
    std::cout << leftPad << RED << "[ERROR] " << msg << RESET << std::endl;
}

void printWarning(const std::string& msg) {
    std::cout << leftPad << YELLOW << "[WARNING] " << msg << RESET << std::endl;
}

void animateText(const std::string& text, int delay) {
    std::cout << leftPad;
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void showProgressBar(int durationInMs) {
    const int total = 30;
    std::cout << leftPad << CYAN << "[";
    for (int i = 0; i <= total; ++i) {
        std::cout << "#" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(durationInMs / total));
    }
    std::cout << "]" << RESET << "\n";
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}


// int main(){
//     printDivider("Welcome");
//     printSuccess("Test Success Message");
//     printError("Test Error Message");
//     printWarning("Test Warning Message");
//     animateText("Animating Text...", 100);
//     showProgressBar(3000);
// }