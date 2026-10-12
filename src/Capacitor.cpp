/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Capacitor.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for capacitor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Capacitor.hpp"

#include <stdexcept>

Capacitor::Capacitor(
    const std::string& name,
    int node1,
    int node2,
    double capacitance
)
    : Component(name, node1, node2, capacitance) {
    if (capacitance <= 0.0) {
        throw std::invalid_argument(
            "Capacitance must be greater than zero."
        );
    }
}

double Capacitor::getCapacitance() const {
    return getValue();
}

double Capacitor::getCurrent(
    double voltageRateOfChange
) const {
    return getValue() * voltageRateOfChange;
}

std::string Capacitor::getType() const {
    return "Capacitor";
}

/*
[ X ] Reject zero or negative capacitance.
[ X ] Implement getCurrent().
[ X ] Decide how capacitors behave during DC analysis.
[ X ] Add initial voltage later if transient analysis requires it.
*/