#ifndef CLI_HPP
#define CLI_HPP

#include "Circuit.hpp"

#include <optional>
#include <string>

class CLI {
public:
    void run();

private:
    bool running = true;
    std::optional<Circuit> circuit;
    std::string currentNetlistFilename;

    void clearScreen() const;
    void showHeader(const std::string& pageTitle) const;
    void showWelcome() const;
    void showMainMenu() const;
    std::string promptForNetlist() const;
    void loadNetlist();
    void showComponentList() const;
    void waitForEnter() const;
    void analysisMenu();
    void runDCAnalysis();
    void runTransientAnalysis();
    void runBangBangControllerSimulation();
};

#endif
