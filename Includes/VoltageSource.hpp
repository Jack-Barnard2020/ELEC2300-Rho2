/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: VoltageSource.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for voltage source functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Component.hpp"

class VoltageSource : public Component {
private:
    double frequency;
    double phase;
    bool isAC;

public:
    // DC voltage source.
    VoltageSource(
        const std::string& name,
        int positiveNode,
        int negativeNode,
        double voltage
    );

    // Sinusoidal AC voltage source.
    VoltageSource(
        const std::string& name,
        int positiveNode,
        int negativeNode,
        double amplitude,
        double frequency,
        double phase
    );

    double getVoltage(double time = 0.0) const;
    double getFrequency() const;
    double getPhase() const;
    bool getIsAC() const;

    std::string getType() const override;
};