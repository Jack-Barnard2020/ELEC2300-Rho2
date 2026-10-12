#pragma once

#include "Component.hpp"

#include <string>

// Ideal capacitor whose capacitance is stored in farads.
class Capacitor final : public Component {
public:
    Capacitor(const std::string& name, int node1, int node2, double capacitance);

    [[nodiscard]] double getCapacitance() const;

    // Returns current in amperes using i = C(dV/dt).
    [[nodiscard]] double getCurrent(double voltageRateOfChange) const;
    [[nodiscard]] std::string getType() const override;
};
