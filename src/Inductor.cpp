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
#include <stdexcept>

Inductor::Inductor(
    const std::string& name,
    int node1,
    int node2,
    double inductance
)
    : Component(name, node1, node2, inductance) {
    // A non-positive inductance is not meaningful for this model.
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
    // Inductor constitutive relation: v = L(di/dt).
    return getValue() * currentRateOfChange;
}

std::string Inductor::getType() const {
    return "Inductor";
}

