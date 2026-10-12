#pragma once

#include <functional>
#include <vector>

// Solves first-order ordinary differential equations numerically.
class FirstOrderODESolver {
public:
    using ODEFunction = std::function<double(double, double)>;

    FirstOrderODESolver() = default;

    // Generates samples using the explicit Euler method.
    void solveEuler(
        const ODEFunction& derivative,
        double initialX,
        double initialY,
        double stepSize,
        double finalX
    );

    [[nodiscard]] const std::vector<double>& getXValues() const;
    [[nodiscard]] const std::vector<double>& getYValues() const;
    void clear();

private:
    std::vector<double> xValues;
    std::vector<double> yValues;
};
