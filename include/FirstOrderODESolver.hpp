/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: FirstOrderODESolver.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for solving first-order ordinary differential equations (ODEs) using numerical methods.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include <functional>
#include <vector>

class FirstOrderODESolver {
public:
    using ODEFunction =
        std::function<double(double, double)>;

private:
    std::vector<double> xValues;
    std::vector<double> yValues;

public:
    FirstOrderODESolver() = default;

    void solveEuler(
        const ODEFunction& derivative,
        double initialX,
        double initialY,
        double stepSize,
        double finalX
    );

    const std::vector<double>&
    getXValues() const;

    const std::vector<double>&
    getYValues() const;

    void clear();
};