/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: VoltageSource.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for voltage source functionality.
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

    // TODO: Validate the frequency.
}

double VoltageSource::getVoltage(double time) const {
    if (!isAC) {
        return value;
    }

    // TODO:
    // Return amplitude * sin(2*pi*frequency*time + phase).
    return 0.0;
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
[ ] Reject negative frequency.
[ ] Implement the sinusoidal getVoltage() calculation.
[ ] Document that phase is measured in radians.
[ ] Decide whether amplitude means peak or RMS voltage.
[ ] Decide how an AC source behaves in DC analysis.
*/