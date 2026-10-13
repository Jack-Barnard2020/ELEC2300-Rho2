/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: main.cpp
    Author: Jack Barnard
    Date: 2026/10/12
    Description: Entry point for the circuit simulator.
   ====================================== */

#include "CLI.hpp"

#include <exception>
#include <iostream>

int main()
{
    try {
        CLI interface;
        interface.run();
    }
    catch (const std::exception& error) {
        std::cerr
            << "Fatal error: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}
