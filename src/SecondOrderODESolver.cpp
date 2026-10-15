/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Resistor.cpp
    Author: Jack Barnard
    Date: 2026/10/15
    Description: Implementation file for second-order ODE solver functionality.
    Change Log:
        2026/10/15- Initial commit (Jack Barnard)
   ====================================== */

#include "SecondOrderODESolver.hpp"

#include <cmath>
#include <stdexcept>

void SecondOrderODESolver::solveEuler(
    const CoupledEquations& equations,
    double initialTime,
    double initialFirstState,
    double initialSecondState,
    double timeStep,
    double stopTime
)
{
    if (!equations) {
        throw std::invalid_argument("Differential equations are not set.");
    }

    if (!std::isfinite(initialTime) ||
        !std::isfinite(initialFirstState) ||
        !std::isfinite(initialSecondState) ||
        !std::isfinite(timeStep) ||
        !std::isfinite(stopTime)) {
        throw std::invalid_argument("Solver values must be finite.");
    }

    if (timeStep <= 0.0) {
        throw std::invalid_argument("Time step must be greater than zero.");
    }

    if (stopTime < initialTime) {
        throw std::invalid_argument("Stop time must not be before initial time.");
    }

    timeValues.clear();
    firstStateValues.clear();
    secondStateValues.clear();

    double time = initialTime;
    double firstState = initialFirstState;
    double secondState = initialSecondState;

    while (time <= stopTime + timeStep * 0.5) {
        timeValues.push_back(time);
        firstStateValues.push_back(firstState);
        secondStateValues.push_back(secondState);

        double firstDerivative = 0.0;
        double secondDerivative = 0.0;

        equations(
            time,
            firstState,
            secondState,
            firstDerivative,
            secondDerivative
        );

        // The RLC states are coupled, so update them within the same step.
        const double nextSecondState =
            secondState + timeStep * secondDerivative;

        double updatedFirstDerivative = 0.0;
        double unusedSecondDerivative = 0.0;

        equations(
            time + timeStep,
            firstState,
            nextSecondState,
            updatedFirstDerivative,
            unusedSecondDerivative
        );

        const double nextFirstState =
            firstState + timeStep * updatedFirstDerivative;

        firstState = nextFirstState;
        secondState = nextSecondState;
        time += timeStep;
    }
}

const std::vector<double>& SecondOrderODESolver::getTimeValues() const
{
    return timeValues;
}

const std::vector<double>& SecondOrderODESolver::getFirstStateValues() const
{
    return firstStateValues;
}

const std::vector<double>& SecondOrderODESolver::getSecondStateValues() const
{
    return secondStateValues;
}
