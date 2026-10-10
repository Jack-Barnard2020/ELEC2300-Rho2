/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Inductor.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for inductor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Component.hpp"

class Inductor : public Component {
public:
    Inductor(
        const std::string& name,
        int node1,
        int node2,
        double inductance
    );

    double getInductance() const;

    // Uses V = L(dI/dt).
    double getVoltage(double currentRateOfChange) const;

    std::string getType() const override;
};