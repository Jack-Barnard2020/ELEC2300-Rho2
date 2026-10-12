#pragma once

#include <string>

class CLI {
public:
    CLI() = default;

    void run();

private:
    // Terminal rendering
    void clearScreen() const;
    void showWelcome() const;
    void showMainMenu() const;

    // User input
    std::string promptForNetlist() const;

    bool running = true;
};
