/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Component.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for component functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Component.hpp"
#include <stdexcept>

Component::Component(
    const std::string& name,
    int node1,
    int node2,
    double value
)
    : name(name),
      node1(node1),
      node2(node2),
      value(value) {
    
    // Every component needs a stable identifier for lookup and diagnostics.
    if (name.empty()) {
        throw std::invalid_argument("Component name cannot be empty.");
    }

    // Node zero is ground; negative node numbers are invalid.
    if (node1 < 0 || node2 < 0) {
        throw std::invalid_argument("Node numbers must be non-negative.");
    }

    // A two-terminal component must connect two distinct nodes.
    if (node1 == node2) {
        throw std::invalid_argument("Node1 and Node2 cannot be the same.");
    }


}

const std::string& Component::getName() const {
    return name;
}

int Component::getNode1() const {
    return node1;
}

int Component::getNode2() const {
    return node2;
}

double Component::getValue() const {
    return value;
}

