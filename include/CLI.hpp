#pragma once

#include "Circuit.hpp"

#include <optional>
#include <string>

class CLI {
public:
    CLI() = default;

    void run();

private:
    // Terminal rendering
    void clearScreen() const;
    void showHeader(const std::string& pageTitle) const;
    void showWelcome() const;
    void showMainMenu() const;
    void showComponentList() const;
    
    // Netlist operations
    void loadNetlist();
    std::string promptForNetlist() const;

    // Input helpers
    void waitForEnter() const;

    // Empty until a valid netlist has been loaded
    std::optional<Circuit> circuit;
    
    // Displayed after successful netlist load
    std::string currentNetlistFilename;

    bool running = true;
};
