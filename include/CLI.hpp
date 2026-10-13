#pragma once

#include "Circuit.hpp"

#include <optional>
#include <string>

class CLI {
private:
    // Terminal rendering
    void clearScreen() const;
    void showHeader(const std::string& pageTitle) const;
    void showWelcome() const;
    void showMainMenu() const;
    void showComponentList() const;

    // Netlist handling
    void loadNetlist();
    std::string promptForNetlist() const;

    // Analysis
    void analysisMenu();
    void runDCAnalysis();
    void runTransientAnalysis();

    // General input
    void waitForEnter() const;

    bool running = true;

    std::optional<Circuit> circuit;
    std::string currentNetlistFilename;
public:
    CLI() = default;

    void run();
};