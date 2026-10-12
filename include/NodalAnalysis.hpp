#pragma once

#include "Circuit.hpp"

#include <vector>

// Builds and solves the modified nodal-analysis system for a circuit.
class NodalAnalysis {
public:
    // The referenced circuit must outlive this analyser.
    explicit NodalAnalysis(const Circuit& circuit);

    [[nodiscard]] std::vector<double> solve();
    [[nodiscard]] const std::vector<double>& getNodeVoltages() const;

private:
    void createSystem();
    void addResistors();
    void stampResistor(int node1, int node2, double conductance);
    void addVoltageSources();
    void handleCapacitors();
    void handleInductors();
    void solveSystem();

    const Circuit& circuit;
    std::vector<std::vector<double>> matrix;
    std::vector<double> rightHandSide;
    std::vector<double> nodeVoltages;
};
