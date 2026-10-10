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

    // TODO: Check that inductance is greater than zero.
}

double Inductor::getInductance() const {
    return value;
}

double Inductor::getVoltage(
    double currentRateOfChange
) const {
    // TODO: Return L * dI/dt.
    return 0.0;
}

std::string Inductor::getType() const {
    return "Inductor";
}

/*
[ ] Reject zero or negative inductance.
[ ] Implement getVoltage().
[ ] Decide how inductors behave during DC analysis.
[ ] Add initial current later if transient analysis requires it.
*/