/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Component.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for component functionality.

    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include <string>

class Component {
protected:
    std::string name;
    int node1;
    int node2;
    double value;

public:
    Component(
        const std::string& name,
        int node1,
        int node2,
        double value
    );

    virtual ~Component() = default;

    const std::string& getName() const;
    int getNode1() const;
    int getNode2() const;
    double getValue() const;

    // Every child class should identify its component type.
    virtual std::string getType() const = 0;
};