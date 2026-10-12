/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: CommandLineInterface.cpp
    Author: Jack Barnard
    Date: 2026/10/12
    Description: Implementation file for the CLI
    Change Log:
        2026/10/12 - Initial commit (Jack Barnard)
   ====================================== */

#include "CLI.hpp"

#include <iostream>
#include <string>

namespace {
    constexpr const char* RESET = "\033[0m";
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* RED = "\033[31m";
}

void CLI::run() {
    showWelcome();

    while (running) {
        showMainMenu();

        std::string choice;

        std::cout << "\nSelect an option: ";
        std::getline(std::cin, choice);

        if (choice == "1") {
            const std::string filename = promptForNetlist();

            clearScreen();

            std::cout 
                << BOLD << CYAN
                << "CIRCUIT SIMULATOR\n"
                << RESET
                << "\n";

            std::cout
                << "Selected netlist: "
                << GREEN
                << filename
                << RESET
                << "\n\n";

            std::cout
                << "Netlist loading will be connected next.\n\n";

            std::cout
                << "Press Enter to return to the main menu...";

            std::cin.get();
        } else if (choice == "2") {
            running = false;
        } else {
            std::cout
                << RED
                << "Invalid option. Please try again."
                << RESET
                << "\n\n";
            
            std::cin.get();
        }
    }

    clearScreen();

    std::cout
        << BOLD
        << "Thank you for using the Circuit Simulator!"
        << RESET
        << "\n";
}

void CLI::clearScreen() const {
    std::cout << "\033[2J\033[1;1H";
}

void CLI::showWelcome() const
{
    clearScreen();

    std::cout
        << CYAN << BOLD
        << "============================================================\n"
        << "                    CIRCUIT SIMULATOR\n"
        << "============================================================\n"
        << RESET
        << '\n';

    std::cout
        << "ELEC2300 Circuit Analysis Tool\n\n"
        << "This program allows a circuit to be loaded from a netlist\n"
        << "and analysed from the command line.\n\n";

    std::cout
        << "Press Enter to continue...";

    std::cin.get();
}

void CLI::showMainMenu() const
{
    clearScreen();

    std::cout
        << CYAN << BOLD
        << "============================================================\n"
        << "                    CIRCUIT SIMULATOR\n"
        << "============================================================\n"
        << RESET;

    std::cout
        << "\nMAIN MENU\n"
        << "------------------------------------------------------------\n\n"

        << "  1. Load netlist\n"
        << "  2. Quit\n";
}


std::string CLI::promptForNetlist() const
{
    clearScreen();

    std::cout
        << CYAN << BOLD
        << "============================================================\n"
        << "                    CIRCUIT SIMULATOR\n"
        << "============================================================\n"
        << RESET;

    std::cout
        << "\nLOAD NETLIST\n"
        << "------------------------------------------------------------\n\n";

    std::cout
        << "Enter the netlist filename or path.\n\n"
        << "> ";

    std::string filename;
    std::getline(std::cin, filename);

    return filename;
}