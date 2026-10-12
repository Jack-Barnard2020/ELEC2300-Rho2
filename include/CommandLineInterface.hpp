#pragma once

#include <string>

class CLI {
    private:
        void clearScreen();
        void showWelcome();
        void showMainMenu();

        std::string promptForNetlist() const;

        bool running = true;
    public:
        CLI() = default;

        void run();
};