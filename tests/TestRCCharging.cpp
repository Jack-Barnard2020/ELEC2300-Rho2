/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: TestRCCharging.cpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Tests a simple DC RC charging circuit and outputs the
                 capacitor-voltage waveform using the terminal plotter.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "FirstOrderODESolver.hpp"
#include "Plotter.hpp"

#include <cmath>
#include <iostream>
#include <vector>

int main() {
    // Circuit values:
    //
    //       R = 1 kohm
    //  +5 V ---/\/\/\---+--- Vc(t)
    //                   |
    //                 C = 100 uF
    //                   |
    //                  GND
    //
    // The capacitor starts discharged: Vc(0) = 0 V.

    constexpr double supplyVoltage = 5.0;
    constexpr double resistance = 1000.0;
    constexpr double capacitance = 100e-6;
    constexpr double initialVoltage = 0.0;
    constexpr double stepSize = 0.0005;
    constexpr double runTime = 0.5;

    const double timeConstant = resistance * capacitance;

    FirstOrderODESolver solver;

    // RC charging equation:
    //
    // dVc/dt = (Vs - Vc) / (R * C)
    solver.solveEuler(
        [=](double /* time */, double capacitorVoltage) {
            return (supplyVoltage - capacitorVoltage)
                / (resistance * capacitance);
        },
        0.0,
        initialVoltage,
        stepSize,
        runTime
    );

    const std::vector<double>& time = solver.getXValues();
    const std::vector<double>& capacitorVoltage = solver.getYValues();

    // Calculate the exact solution so that the numerical result can be
    // checked and both waveforms can be displayed together.
    std::vector<double> exactVoltage;
    exactVoltage.reserve(time.size());

    for (double currentTime : time) {
        exactVoltage.push_back(
            supplyVoltage
            * (1.0 - std::exp(-currentTime / timeConstant))
        );
    }

    Series eulerSeries;
    eulerSeries.name = "Euler approximation";
    eulerSeries.y = capacitorVoltage;
    eulerSeries.symbol = '*';
    eulerSeries.color = PlotColor::Cyan;

    Series exactSeries;
    exactSeries.name = "Exact RC response";
    exactSeries.y = exactVoltage;
    exactSeries.symbol = '+';
    exactSeries.color = PlotColor::Yellow;

    Plot plot(90, "s", "V");

    std::cout << "DC RC charging-circuit test\n";
    std::cout << "Supply voltage: " << supplyVoltage << " V\n";
    std::cout << "Resistance: " << resistance << " ohm\n";
    std::cout << "Capacitance: " << capacitance << " F\n";
    std::cout << "Time constant: " << timeConstant << " s\n\n";

    std::cout << plot.plot(
        time,
        std::vector<Series>{eulerSeries, exactSeries}
    );

    const double numericalFinalVoltage = capacitorVoltage.back();
    const double exactFinalVoltage = exactVoltage.back();
    const double finalError = std::abs(
        numericalFinalVoltage - exactFinalVoltage
    );

    std::cout << "\nFinal numerical voltage: "
              << numericalFinalVoltage << " V\n";
    std::cout << "Final exact voltage: "
              << exactFinalVoltage << " V\n";
    std::cout << "Absolute error: " << finalError << " V\n";

    // With a 1 ms step and a 0.1 s time constant, Euler's method should
    // comfortably meet this tolerance.
    constexpr double tolerance = 0.01;

    if (finalError > tolerance) {
        std::cerr << "RC charging test failed.\n";
        return 1;
    }

    std::cout << "RC charging test passed.\n";
    return 0;
}
