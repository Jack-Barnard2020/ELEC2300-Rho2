/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Circuit.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for circuit functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Component.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Circuit {
private:
    std::vector<std::unique_ptr<Component>> components;
    int highestNode;

public:
    Circuit();

    void addComponent(
        std::unique_ptr<Component> component
    );

    const std::vector<std::unique_ptr<Component>>&
    getComponents() const;

    const Component* getComponentByName(
        const std::string& name
    ) const;

    std::size_t getNumberOfComponents() const;
    int getHighestNode() const;

private:
    void updateHighestNode(
        const Component& component
    );
};