/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Circuit.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for circuit functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */
#include "Circuit.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

Circuit::Circuit() : highestNode(0) {}

void Circuit::addComponent(std::unique_ptr<Component> component) {
    if (!component) {
        throw std::invalid_argument("Cannot add a null component.");
    }
    if (std::find_if(components.begin(), components.end(),
        [&component](const std::unique_ptr<Component>& current) {
            return current->getName() == component->getName();
        }) != components.end()) {
        throw std::invalid_argument("Cannot add a component with a duplicate name.");
    }
    updateHighestNode(*component);
    components.push_back(std::move(component));
}

const std::vector<std::unique_ptr<Component>>& Circuit::getComponents() const {
    return components;
}

const Component* Circuit::getComponentByName(const std::string& requestedName) const {
    for (const auto& component : components) {
        if (component->getName() == requestedName) {
            return component.get();
        }
    }
    return nullptr;
}

std::size_t Circuit::getNumberOfComponents() const { return components.size(); }
int Circuit::getHighestNode() const { return highestNode; }

void Circuit::updateHighestNode(const Component& component) {
    highestNode = std::max({highestNode, component.getNode1(), component.getNode2()});
}
