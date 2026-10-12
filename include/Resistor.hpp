#pragma once

#include "Component.hpp"

#include <string>

// Ideal resistor whose resistance is stored in ohms.
class Resistor final : public Component {
public:
    Resistor(const std::string& name, int node1, int node2, double resistance);

    [[nodiscard]] double getResistance() const;
    [[nodiscard]] double getConductance() const;
    [[nodiscard]] double getCurrent(double voltage) const;
    [[nodiscard]] std::string getType() const override;
};
