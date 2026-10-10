/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Capacitor.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for capacitor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Capacitor.hpp"

Capacitor::Capacitor(
    const std::string& name,
    int node1,
    int node2,
    double capacitance
)
    : Component(name, node1, node2, capacitance) {

    // Reject zero or negative capacitance.
    if (capacitance <= 0.0) {
        throw std::invalid_argument ("Capacitance must be greater than zero.");
    }
}

double Capacitor::getCapacitance() const {
    return value;
}

double Capacitor::getCurrent(
    double voltageRateOfChange
) const {
    // TODO: Return C * dV/dt.
    return 0.0;
}

std::string Capacitor::getType() const {
    return "Capacitor";
}

/*
[ X ] Reject zero or negative capacitance.
[ ] Implement getCurrent().
[ ] Decide how capacitors behave during DC analysis.
[ ] Add initial voltage later if transient analysis requires it.
*/