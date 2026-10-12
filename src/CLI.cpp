/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: CLI.cpp
    Author: Jack Barnard
    Date: 2026/10/12
    Description: Implementation file for the command-line interface.
    Change Log:
        2026/10/12 - Initial commit (Jack Barnard)
   ====================================== */

#include "CLI.hpp"
#include "ReadNetlist.hpp"
#include "VoltageSource.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

namespace {

constexpr const char* RESET  = "\033[0m";
constexpr const char* BOLD   = "\033[1m";
constexpr const char* CYAN   = "\033[36m";
constexpr const char* GREEN  = "\033[32m";
constexpr const char* RED    = "\033[31m";
constexpr const char* YELLOW = "\033[33m";

} // namespace


void CLI::run()
{
    showWelcome();
    waitForEnter();

    while (running) {
        showMainMenu();

        std::string choice;

        std::cout << "\nSelect an option: ";
        std::getline(std::cin, choice);

        if (choice == "1") {
            loadNetlist();
        }
        else if (choice == "2") {
            running = false;
        }
        else {
            std::cout
                << RED
                << "\nInvalid option. Please enter 1 or 2.\n"
                << RESET;

            waitForEnter();
        }
    }

    showHeader("Goodbye");

    std::cout
        << "Thank you for using the circuit simulator.\n\n";
}


void CLI::clearScreen() const
{
    // Clear the terminal and move the cursor back to the top-left.
    std::cout << "\033[2J\033[H" << std::flush;
}


void CLI::showHeader(const std::string& pageTitle) const
{
    clearScreen();

    std::cout
        << BOLD
        << CYAN
        << "============================================================\n"
        << "                    CIRCUIT SIMULATOR\n"
        << "============================================================\n"
        << RESET;

    if (!pageTitle.empty()) {
        std::cout
            << '\n'
            << BOLD
            << pageTitle
            << RESET
            << '\n'
            << "------------------------------------------------------------\n";
    }

    if (!currentNetlistFilename.empty()) {
        std::cout
            << "Current netlist: "
            << GREEN
            << currentNetlistFilename
            << RESET
            << '\n'
            << "------------------------------------------------------------\n";
    }

    std::cout << '\n';
}


void CLI::showWelcome() const
{
    showHeader("Welcome");

    std::cout
        << "This program loads an electrical circuit from a netlist\n"
        << "and provides command-line circuit analysis.\n";
}


void CLI::showMainMenu() const
{
    showHeader("Main Menu");

    std::cout
        << "1. Load a netlist\n"
        << "2. Quit\n";
}


std::string CLI::promptForNetlist() const
{
    showHeader("Load Netlist");

    std::cout
        << "Enter the netlist filename or path.\n"
        << "Enter Q to cancel and return to the main menu.\n\n"
        << "> ";

    std::string filename;
    std::getline(std::cin, filename);

    return filename;
}


void CLI::loadNetlist()
{
    const std::string filename = promptForNetlist();

    if (filename == "q" || filename == "Q") {
        return;
    }

    if (filename.empty()) {
        std::cout
            << RED
            << "\nNo filename entered.\n"
            << RESET;

        waitForEnter();
        return;
    }

    showHeader("Loading Netlist");

    std::cout
        << "Reading "
        << filename
        << "...\n"
        << std::flush;

    try {
        // Parse the netlist and construct a new circuit.
        ReadNetlist reader(filename);
        Circuit loadedCircuit = reader.read();

        /*
         * Only replace the currently loaded circuit once the new
         * netlist has been read successfully.
         */
        circuit = std::move(loadedCircuit);
        currentNetlistFilename = filename;

        showComponentList();
    }
    catch (const std::exception& error) {
        showHeader("Error Loading Netlist");

        std::cout
            << RED
            << "The netlist could not be loaded.\n\n"
            << RESET
            << error.what()
            << '\n';

        waitForEnter();
    }
}


void CLI::showComponentList() const
{
    showHeader("Component List");

    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    const auto& components = circuit->getComponents();

    std::cout
        << GREEN
        << "Netlist loaded successfully.\n"
        << RESET
        << '\n';

    // Table headings.
    std::cout
        << std::left
        << std::setw(5)  << "No."
        << std::setw(12) << "Name"
        << std::setw(16) << "Type"
        << std::setw(8)  << "Node 1"
        << std::setw(8)  << "Node 2"
        << std::setw(14) << "Value"
        << std::setw(16) << "Frequency"
        << "Phase\n";

    std::cout
        << "---------------------------------------------------------------------------------------\n";

    for (std::size_t index = 0;
         index < components.size();
         ++index) {

        const Component& component = *components.at(index);

        std::cout
            << std::left
            << std::setw(5)  << index + 1
            << std::setw(12) << component.getName()
            << std::setw(16) << component.getType()
            << std::setw(8)  << component.getNode1()
            << std::setw(8)  << component.getNode2()
            << std::setw(14) << component.getValue();

        /*
         * Voltage sources may contain additional waveform
         * information which is not part of the base Component class.
         */
        const VoltageSource* voltageSource =
            dynamic_cast<const VoltageSource*>(&component);

        if (voltageSource != nullptr) {

            /*
             * Replace these getter names if your VoltageSource.hpp
             * uses different names.
             */
            if (voltageSource->getFrequency() != 0.0) {
                std::cout
                    << std::setw(16)
                    << (std::to_string(voltageSource->getFrequency()) + " Hz")
                    << voltageSource->getPhase()
                    << " deg";
            }
            else {
                // DC voltage source.
                std::cout
                    << std::setw(16) << "-"
                    << "-";
            }
        }
        else {
            // R, L and C components have no source frequency/phase.
            std::cout
                << std::setw(16) << "-"
                << "-";
        }

        std::cout << '\n';
    }

    std::cout
        << "---------------------------------------------------------------------------------------\n"
        << "Number of components: "
        << circuit->getNumberOfComponents()
        << '\n'
        << "Highest node: "
        << circuit->getHighestNode()
        << '\n';

    waitForEnter();
}


void CLI::waitForEnter() const
{
    std::cout
        << YELLOW
        << "\nPress Enter to continue..."
        << RESET;

    std::cin.get();
}