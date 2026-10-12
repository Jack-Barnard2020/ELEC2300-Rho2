#pragma once

#include "Component.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// Owns the components that make up a circuit and tracks its highest node.
class Circuit {
public:
    Circuit();
    ~Circuit() = default;

    // Components have unique ownership, so circuits are movable but not copyable.
    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;
    Circuit(Circuit&&) noexcept = default;
    Circuit& operator=(Circuit&&) noexcept = default;

    void addComponent(std::unique_ptr<Component> component);

    [[nodiscard]] const std::vector<std::unique_ptr<Component>>& getComponents() const;
    [[nodiscard]] const Component* getComponentByName(const std::string& name) const;
    [[nodiscard]] std::size_t getNumberOfComponents() const;
    [[nodiscard]] int getHighestNode() const;

private:
    void updateHighestNode(const Component& component);

    std::vector<std::unique_ptr<Component>> components;
    int highestNode;
};
