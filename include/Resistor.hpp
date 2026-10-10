/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Resistor.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for resistor functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Component.hpp"

class Resistor : public Component {
public:
    Resistor(
        const std::string& name,
        int node1,
        int node2,
        double resistance
    );

    double getResistance() const;
    double getConductance() const;
    double getCurrent(double voltage) const;

    std::string getType() const override;
};