/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: fourierSeries.cpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Implementation file for the Fourier series implementation.
    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
   ====================================== */

#include "FourierSeries.hpp"
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

void FourierSeries::generate()
{
    xValues.clear();
    yValues.clear();

    for (double t = 0.0; t <= runTime; t += timeStep) {
        double y = 0.0;

        for (std::size_t i = 0; i < sineCoefficients.size(); i++) {
            int harmonic = i + 1;

            double angle = 2.0 * PI * harmonic * fundamentalFrequency * t;

            y += cosineCoefficients[i] * std::cos(angle)
                + sineCoefficients[i] * std::sin(angle);
        }

        xValues.push_back(t);
        yValues.push_back(y);
    }
}

const std::vector<double>& getXValues() const {
    return xValues;
};

const std::vector<double>& getYValues() const {
    return yValues;
};