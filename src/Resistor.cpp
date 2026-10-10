/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Resistor.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for resistor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Resistor.hpp"

Resistor::Resistor(
    const std::string& name,
    int node1,
    int node2,
    double resistance
)
    : Component(name, node1, node2, resistance) {

    if (resistance <= 0.0) {
        throw std::invalid_argument(
            "Resistance must be greater than zero."
        );
    }
}

double Resistor::getResistance() const {
    return getValue();
}

double Resistor::getConductance() const {
    return 1.0 / getValue();
}

double Resistor::getCurrent(double voltage) const {
    return voltage / getValue();
}

std::string Resistor::getType() const {
    return "Resistor";
}