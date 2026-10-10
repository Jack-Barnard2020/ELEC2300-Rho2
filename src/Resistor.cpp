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
        // Reject zero or negative resistance.
        if (resistance <= 0.0) {
            throw std::invalid_argument (
                "Resistance must be greater than zero."
            );
        }
}

// Getter function for the resistance value.
double Resistor::getResistance() const {
    return resistance;
}

// Getter function for the conductance value.
double Resistor::getConductance() const {
    // Conductance is the reciprocal of resistance.
    return 1.0 / resistance;
}

// Getter function for the current value.
double Resistor::getCurrent(double voltage) const {
    return voltage / resistance;
}

std::string Resistor::getType() const {
    return "Resistor";
}

/*
[X] Reject zero or negative resistance.
[X] Implement getConductance().
[X] Implement getCurrent().
[X] Test Ohm's law with known values.
*/