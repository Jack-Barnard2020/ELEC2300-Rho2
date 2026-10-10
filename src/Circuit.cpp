/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Circuit.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for circuit functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "Circuit.hpp"

#include <algorithm>
#include <utility>

Circuit::Circuit()
    : highestNode(0) {
}

void Circuit::addComponent(
    std::unique_ptr<Component> component
) {
    // TODO: Reject a null component.

    // TODO: Reject duplicate component names.

    // TODO: Validate the component's nodes.

    updateHighestNode(*component);

    components.push_back(std::move(component));
}

const std::vector<std::unique_ptr<Component>>&
Circuit::getComponents() const {
    return components;
}

const Component* Circuit::getComponentByName(
    const std::string& requestedName
) const {
    // TODO: Search through components.

    return nullptr;
}

std::size_t Circuit::getNumberOfComponents() const {
    return components.size();
}

int Circuit::getHighestNode() const {
    return highestNode;
}

void Circuit::updateHighestNode(
    const Component& component
) {
    // TODO:
    // Find the largest of:
    // - current highestNode
    // - component.getNode1()
    // - component.getNode2()
}

/*
[ ] Check for null pointers.
[ ] Reject duplicate component names.
[ ] Implement getComponentByName().
[ ] Implement updateHighestNode().
[ ] Check that ground node 0 exists.
[ ] Decide whether nodes must be consecutive.
[ ] Add a validate() method if needed.
*/