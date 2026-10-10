/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: VoltageSource.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for voltage source functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "VoltageSource.hpp"

#include <cmath>

namespace {
    constexpr double PI = 3.14159265358979323846;
}

VoltageSource::VoltageSource(
    const std::string& name,
    int positiveNode,
    int negativeNode,
    double voltage
)
    : Component(
          name,
          positiveNode,
          negativeNode,
          voltage
      ),
      frequency(0.0),
      phase(0.0),
      isAC(false) {
}

VoltageSource::VoltageSource(
    const std::string& name,
    int positiveNode,
    int negativeNode,
    double amplitude,
    double frequency,
    double phase
)
    : Component(
          name,
          positiveNode,
          negativeNode,
          amplitude
      ),
      frequency(frequency),
      phase(phase),
      isAC(true) {
    if (frequency < 0.0) {
        throw std::invalid_argument(
            "Frequency must be non-negative."
        );
    }
}

double VoltageSource::getVoltage(double time) const {
    if (!isAC) {
        return getValue();
    }

    return getValue()
        * std::sin(
            2.0 * PI * frequency * time + phase
        );
}

double VoltageSource::getFrequency() const {
    return frequency;
}

double VoltageSource::getPhase() const {
    return phase;
}

bool VoltageSource::getIsAC() const {
    return isAC;
}

std::string VoltageSource::getType() const {
    return "VoltageSource";
}


/*
[ X ] Reject negative frequency.
[ X ] Implement the sinusoidal getVoltage() calculation.
[ X ] Document that phase is measured in radians.
[ X ] Decide whether amplitude means peak or RMS voltage.
[ X ] Decide how an AC source behaves in DC analysis.       DC analysis should treat AC sources as open circuits, so the voltage is effectively zero.
*/