#pragma once

#include "Component.hpp"

#include <string>

// Ideal inductor whose inductance is stored in henries.
class Inductor final : public Component {
public:
    Inductor(const std::string& name, int node1, int node2, double inductance);

    [[nodiscard]] double getInductance() const;

    // Returns voltage in volts using v = L(dI/dt).
    [[nodiscard]] double getVoltage(double currentRateOfChange) const;
    [[nodiscard]] std::string getType() const override;
};
