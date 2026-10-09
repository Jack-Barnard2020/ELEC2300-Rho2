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

FourierSeries::FourierSeries(std::vector<double> sineCoefficients,
    std::vector<double> cosineCoefficients,
    double fundamentalFrequency, 
    double timeStep, 
    double runTime) : sineCoefficients(sineCoefficients),
                      cosineCoefficients(cosineCoefficients),
                      fundamentalFrequency(fundamentalFrequency), 
                      timeStep(timeStep), 
                      runTime(runTime)
{}

void FourierSeries::generate()
{
    xValues.clear();
    yValues.clear();

    for (double t = 0.0; t <= runTime; t += timeStep) {
        double y = 0.0;

        for (std::size_t i = 0; i < sineCoefficients.size(); ++i) {
            int harmonic = i + 1;

            double angle =
                2.0 * std::acos(-1.0)
                * harmonic
                * fundamentalFrequency
                * t;

            if (i < cosineCoefficients.size()) {
                y += cosineCoefficients[i] * std::cos(angle);
            }

            y += sineCoefficients[i] * std::sin(angle);
        }

        xValues.push_back(t);
        yValues.push_back(y);
    }
}

const std::vector<double>& FourierSeries::getXValues() const {
    return xValues;
}

const std::vector<double>& FourierSeries::getYValues() const {
    return yValues;
}