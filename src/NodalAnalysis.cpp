/* =========== ELEC2300 - Rho2 *=========
    Project: Circuit Sim*lator
    File: NodalAnalysis.hpp
*   Author: Jack Barnard
    Date: *026/10/10
    Description: Header file for nodal analysis functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */
   
#include "NodalAnalysis.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"
#include "Resistor.hpp"
#include "VoltageSource.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
constexpr double pivotTolerance = 1e-12;
}

NodalAnalysis::NodalAnalysis(const Circuit& circuit) : circuit(circuit) {}

std::vector<double> NodalAnalysis::solve() {
    createSystem();
    addResistors();
    addVoltageSources();
    handleCapacitors();
    handleInductors();
    solveSystem();
    return nodeVoltages;
}

const std::vector<double>& NodalAnalysis::getNodeVoltages() const { return nodeVoltages; }

void NodalAnalysis::createSystem() {
    const std::size_t nodeUnknowns = static_cast<std::size_t>(circuit.getHighestNode());
    std::size_t extraUnknowns = 0;
    for (const auto& component : circuit.getComponents()) {
        if (const auto* source = dynamic_cast<const VoltageSource*>(component.get())) {
            if (!source->getIsAC()) ++extraUnknowns;
        } else if (dynamic_cast<const Inductor*>(component.get()) != nullptr) {
            ++extraUnknowns;
        }
    }
    const std::size_t total = nodeUnknowns + extraUnknowns;
    if (total == 0) throw std::runtime_error("Cannot analyse an empty circuit.");
    matrix.assign(total, std::vector<double>(total, 0.0));
    rightHandSide.assign(total, 0.0);
    nodeVoltages.assign(nodeUnknowns, 0.0); // Ground node 0 is not included.
}

void NodalAnalysis::addResistors() {
    for (const auto& component : circuit.getComponents()) {
        if (const auto* resistor = dynamic_cast<const Resistor*>(component.get())) {
            stampResistor(resistor->getNode1(), resistor->getNode2(), resistor->getConductance());
        }
    }
}

void NodalAnalysis::stampResistor(int node1, int node2, double conductance) {
    if (node1 != 0) matrix[node1 - 1][node1 - 1] += conductance;
    if (node2 != 0) matrix[node2 - 1][node2 - 1] += conductance;
    if (node1 != 0 && node2 != 0) {
        matrix[node1 - 1][node2 - 1] -= conductance;
        matrix[node2 - 1][node1 - 1] -= conductance;
    }
}

void NodalAnalysis::addVoltageSources() {
    std::size_t sourceIndex = nodeVoltages.size();
    for (const auto& component : circuit.getComponents()) {
        const auto* source = dynamic_cast<const VoltageSource*>(component.get());
        if (source == nullptr || source->getIsAC()) continue; // AC sources are inactive in DC analysis.
        const int positive = source->getNode1();
        const int negative = source->getNode2();
        if (positive != 0) matrix[positive - 1][sourceIndex] = matrix[sourceIndex][positive - 1] = 1.0;
        if (negative != 0) matrix[negative - 1][sourceIndex] = matrix[sourceIndex][negative - 1] = -1.0;
        rightHandSide[sourceIndex] = source->getVoltage(0.0);
        ++sourceIndex;
    }
}

void NodalAnalysis::handleCapacitors() {
    // Capacitors are open circuits at DC steady state, so no matrix stamp is required.
}

void NodalAnalysis::handleInductors() {
    std::size_t sourceIndex = nodeVoltages.size();
    for (const auto& component : circuit.getComponents()) {
        if (const auto* source = dynamic_cast<const VoltageSource*>(component.get())) {
            if (!source->getIsAC()) ++sourceIndex;
        }
    }
    for (const auto& component : circuit.getComponents()) {
        const auto* inductor = dynamic_cast<const Inductor*>(component.get());
        if (inductor == nullptr) continue;
        const int node1 = inductor->getNode1();
        const int node2 = inductor->getNode2();
        if (node1 != 0) matrix[node1 - 1][sourceIndex] = matrix[sourceIndex][node1 - 1] = 1.0;
        if (node2 != 0) matrix[node2 - 1][sourceIndex] = matrix[sourceIndex][node2 - 1] = -1.0;
        ++sourceIndex; // RHS is zero, modelling a DC short circuit.
    }
}

void NodalAnalysis::solveSystem() {
    const std::size_t n = matrix.size();
    for (std::size_t column = 0; column < n; ++column) {
        std::size_t pivot = column;
        for (std::size_t row = column + 1; row < n; ++row) {
            if (std::abs(matrix[row][column]) > std::abs(matrix[pivot][column])) pivot = row;
        }
        if (std::abs(matrix[pivot][column]) < pivotTolerance) {
            throw std::runtime_error("Circuit matrix is singular; check for floating nodes or conflicting ideal sources.");
        }
        std::swap(matrix[column], matrix[pivot]);
        std::swap(rightHandSide[column], rightHandSide[pivot]);
        for (std::size_t row = column + 1; row < n; ++row) {
            const double factor = matrix[row][column] / matrix[column][column];
            matrix[row][column] = 0.0;
            for (std::size_t k = column + 1; k < n; ++k) matrix[row][k] -= factor * matrix[column][k];
            rightHandSide[row] -= factor * rightHandSide[column];
        }
    }

    std::vector<double> solution(n, 0.0);
    for (std::size_t reverse = n; reverse-- > 0;) {
        double value = rightHandSide[reverse];
        for (std::size_t column = reverse + 1; column < n; ++column) value -= matrix[reverse][column] * solution[column];
        solution[reverse] = value / matrix[reverse][reverse];
    }
    std::copy_n(solution.begin(), nodeVoltages.size(), nodeVoltages.begin());
}
