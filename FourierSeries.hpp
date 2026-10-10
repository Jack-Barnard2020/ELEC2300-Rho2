/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: FourierSeries.hpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Header file for generating Fourier series waveforms.

    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
        2026/10/10 - Improved readability and documentation
   ====================================== */

#pragma once

#include <vector>

// Generates a waveform from sine and cosine Fourier coefficients.
//
// Coefficient index 0 represents the fundamental frequency;
// index 1 represents the second harmonic, and so on.
class FourierSeries {
private:
    std::vector<double> sineCoefficients;
    std::vector<double> cosineCoefficients;

    double fundamentalFrequency;
    double timeStep;
    double runTime;

    // Generated time and amplitude values.
    std::vector<double> xValues;
    std::vector<double> yValues;

public:
    FourierSeries(
        std::vector<double> sineCoefficients,
        std::vector<double> cosineCoefficients,
        double fundamentalFrequency,
        double timeStep,
        double runTime
    );

    // Generate the waveform using the supplied Fourier coefficients.
    void generate();

    // Return references to the generated data without copying it.
    const std::vector<double>& getXValues() const;
    const std::vector<double>& getYValues() const;
};
