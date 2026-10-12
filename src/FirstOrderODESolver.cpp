/* =========== ELEC2300 -*Rho2 ==========
    Project: Circu*t Simulator
    File: FirstOrderOD*Solver.hpp
    Author: Jack Barnar*
    Date: 2026/10/10
    Descript*on: Header file for first-order OD* solver functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */
   
#include "FirstOrderODESolver.hpp"

#include <algorithm>
#include <stdexcept>

void FirstOrderODESolver::solveEuler(
    const ODEFunction& derivative,
    double initialX,
    double initialY,
    double stepSize,
    double finalX
) {
    if (!derivative) {
        throw std::invalid_argument("Derivative function must be valid.");
    }
    if (stepSize <= 0.0) {
        throw std::invalid_argument("Step size must be greater than zero.");
    }
    if (finalX < initialX) {
        throw std::invalid_argument("finalX must not be less than initialX.");
    }

    clear();
    double x = initialX;
    double y = initialY;
    xValues.push_back(x);
    yValues.push_back(y);

    while (x < finalX) {
        const double actualStep = std::min(stepSize, finalX - x);
        y += actualStep * derivative(x, y);
        x += actualStep;
        xValues.push_back(x);
        yValues.push_back(y);
    }
}

const std::vector<double>& FirstOrderODESolver::getXValues() const { return xValues; }
const std::vector<double>& FirstOrderODESolver::getYValues() const { return yValues; }
void FirstOrderODESolver::clear() { xValues.clear(); yValues.clear(); }
