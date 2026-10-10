/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Inductor.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for inductor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Inductor.hpp"

Inductor::Inductor(
    const std::string& name,
    int node1,
    int node2,
    double inductance
)
    : Component(name, node1, node2, inductance) {
    if (inductance <= 0.0) {
        throw std::invalid_argument(
            "Inductance must be greater than zero."
        );
    }
}

double Inductor::getInductance() const {
    return getValue();
}

double Inductor::getVoltage(
    double currentRateOfChange
) const {
    return getValue() * currentRateOfChange;
}

std::string Inductor::getType() const {
    return "Inductor";
}

/*
[ X ] Reject zero or negative inductance.
[ X ] Implement getVoltage().
[ X ] Decide how inductors behave during DC analysis.
[ X ] Add initial current later if transient analysis requires it.
*/