#pragma once

#include <string>

// Common interface and shared data for every two-terminal circuit component.
class Component {
public:
    Component(const std::string& name, int node1, int node2, double value);
    virtual ~Component() = default;

    Component(const Component&) = default;
    Component& operator=(const Component&) = default;
    Component(Component&&) noexcept = default;
    Component& operator=(Component&&) noexcept = default;

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] int getNode1() const;
    [[nodiscard]] int getNode2() const;
    [[nodiscard]] double getValue() const;

    // Enables callers to identify a derived component through the base interface.
    [[nodiscard]] virtual std::string getType() const = 0;

protected:
    std::string name;
    int node1;
    int node2;
    double value;
};
