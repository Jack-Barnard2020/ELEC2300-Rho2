/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Capacitor.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for capacitor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Component.hpp"

class Capacitor : public Component {
public:
    Capacitor(
        const std::string& name,
        int node1,
        int node2,
        double capacitance
    );

    double getCapacitance() const;

    // Uses i = C(dV/dt).
    double getCurrent(double voltageRateOfChange) const;

    std::string getType() const override;
};