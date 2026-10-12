#pragma once

#include "Component.hpp"

#include <string>

// Ideal DC or sinusoidal AC voltage source.
class VoltageSource final : public Component {
public:
    // Constant DC source. Voltage is measured in volts.
    VoltageSource(const std::string& name, int positiveNode, int negativeNode, double voltage);

    // Sinusoidal source. Amplitude is peak volts, frequency is hertz,
    // and phase is radians.
    VoltageSource(
        const std::string& name,
        int positiveNode,
        int negativeNode,
        double amplitude,
        double frequency,
        double phase
    );

    [[nodiscard]] double getVoltage(double time = 0.0) const;
    [[nodiscard]] double getFrequency() const;
    [[nodiscard]] double getPhase() const;
    [[nodiscard]] bool getIsAC() const;
    [[nodiscard]] std::string getType() const override;

private:
    double frequency;
    double phase;
    bool isAC;
};
