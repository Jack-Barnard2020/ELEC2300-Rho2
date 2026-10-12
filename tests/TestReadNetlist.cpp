/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: TestReadNetlist.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Tests reading and parsing a simple DC RC circuit netlist.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "ReadNetlist.hpp"

#include <iostream>

int main() {
    try {
        ReadNetlist reader("tests/test_rc.net");

        Circuit circuit = reader.read();

        std::cout << "Netlist loaded successfully.\n\n";

        std::cout
            << "Number of components: "
            << circuit.getNumberOfComponents()
            << '\n';

        std::cout
            << "Highest node: "
            << circuit.getHighestNode()
            << "\n\n";

        std::cout << "Components:\n";

        for (const auto& component : circuit.getComponents()) {
            std::cout
                << "  Name: " << component->getName()
                << "\n  Type: " << component->getType()
                << "\n  Node 1: " << component->getNode1()
                << "\n  Node 2: " << component->getNode2()
                << "\n  Value: " << component->getValue()
                << "\n\n";
        }

        // Basic automatic checks.
        if (circuit.getNumberOfComponents() != 3) {
            std::cerr << "FAIL: Expected 3 components.\n";
            return 1;
        }

        if (circuit.getHighestNode() != 2) {
            std::cerr << "FAIL: Expected highest node to be 2.\n";
            return 1;
        }

        if (circuit.getComponentByName("V1") == nullptr) {
            std::cerr << "FAIL: Could not find V1.\n";
            return 1;
        }

        if (circuit.getComponentByName("R1") == nullptr) {
            std::cerr << "FAIL: Could not find R1.\n";
            return 1;
        }

        if (circuit.getComponentByName("C1") == nullptr) {
            std::cerr << "FAIL: Could not find C1.\n";
            return 1;
        }

        std::cout << "All netlist tests passed.\n";

        return 0;
    }
    catch (const std::exception& error) {
        std::cerr
            << "Netlist test failed: "
            << error.what()
            << '\n';

        return 1;
    }
}