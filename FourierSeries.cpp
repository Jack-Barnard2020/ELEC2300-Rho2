/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: fourierSeries.cpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Implementation file for the Fourier series implementation.
    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
   ====================================== */

#include "fourierSeries.hpp"
#include <cmath>

fourierSeries::fourierSeries(std::vector<double> sineCoefficents,
    std::vector<double> cosineCoefficents, 
    double fundamentalFrequency, 
    double timeStep, 
    double runTime) : sineCoefficents(sineCoefficents), 
                      cosineCoefficents(cosineCoefficents), 
                      fundamentalFrequency(fundamentalFrequency), 
                      timeStep(timeStep), 
                      runTime(runTime) {};

void generate() {
    xValues.clear();
    yValues.clear();

    // TODO: Generate x values based on time step and run time
    // TODO: Calculate y values using the Fourier series formula

};

const std::vector<double>& getXValues() const {
    return xValues;
};

const std::vector<double>& getYValues() const {
    return yValues;
};