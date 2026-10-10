/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: FirstOrderODESolver.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Implementation file for solving first-order ordinary differential equations (ODEs) using numerical methods.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "FirstOrderODESolver.hpp"

#include <stdexcept>

void FirstOrderODESolver::solveEuler(
    const ODEFunction& derivative,
    double initialX,
    double initialY,
    double stepSize,
    double finalX
) {
    // TODO: Reject a zero or negative step size.

    // TODO: Check that finalX is valid.

    clear();

    double x = initialX;
    double y = initialY;

    xValues.push_back(x);
    yValues.push_back(y);

    while (x < finalX) {
        /*
         * TODO: Apply Euler's method:
         *
         * yNew = y + stepSize * derivative(x, y)
         * xNew = x + stepSize
         */

        // TODO: Stop the final step going beyond finalX.

        // TODO: Add the new x and y to the vectors.

        // Temporary break prevents an infinite loop
        // until the update code is implemented.
        break;
    }
}

const std::vector<double>&
FirstOrderODESolver::getXValues() const {
    return xValues;
}

const std::vector<double>&
FirstOrderODESolver::getYValues() const {
    return yValues;
}

void FirstOrderODESolver::clear() {
    xValues.clear();
    yValues.clear();
}

/*
[ ] Validate the step size.
[ ] Validate the integration range.
[ ] Implement the Euler update.
[ ] Handle the final partial step.
[ ] Ensure xValues and yValues remain equal in length.
[ ] Test against an ODE with a known solution.
[ ] Add Runge-Kutta fourth order later if required.
*/