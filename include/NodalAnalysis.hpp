/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: NodalAnalysis.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for running nodal analysis functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Circuit.hpp"

#include <vector>

class NodalAnalysis {
private:
    const Circuit& circuit;

    std::vector<std::vector<double>> matrix;
    std::vector<double> rightHandSide;
    std::vector<double> nodeVoltages;

public:
    explicit NodalAnalysis(
        const Circuit& circuit
    );

    std::vector<double> solve();

    const std::vector<double>&
    getNodeVoltages() const;

private:
    void createSystem();
    void addResistors();
    void addVoltageSources();
    void handleCapacitors();
    void handleInductors();
    void solveSystem();

    void stampResistor(
        int node1,
        int node2,
        double conductance
    );
};