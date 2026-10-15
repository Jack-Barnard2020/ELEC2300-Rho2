#pragma once

#include <functional>
#include <vector>

class SecondOrderODESolver {
public:
    using CoupledEquations = std::function<void(
        double time,
        double firstState,
        double secondState,
        double& firstDerivative,
        double& secondDerivative
    )>;

    void solveEuler(
        const CoupledEquations& equations,
        double initialTime,
        double initialFirstState,
        double initialSecondState,
        double timeStep,
        double stopTime
    );

    const std::vector<double>& getTimeValues() const;
    const std::vector<double>& getFirstStateValues() const;
    const std::vector<double>& getSecondStateValues() const;

private:
    std::vector<double> timeValues;
    std::vector<double> firstStateValues;
    std::vector<double> secondStateValues;
};