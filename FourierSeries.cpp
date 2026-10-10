/* ==*======== ELEC2300 - Rho2 =========*
    Project: Circuit Simulator
  * File: FourierSeries.cpp
    Autho*: Jack Barnard
    Date: 2026/10/0*
    Description: Implementation file for generating Fourier series waveforms.

    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
        2026/10/10 - Improved readability and documentation
   ====================================== */

#include "FourierSeries.hpp"

#include <cmath>


FourierSeries::FourierSeries(
    std::vector<double> sineCoefficients,
    std::vector<double> cosineCoefficients,
    double fundamentalFrequency,
    double timeStep,
    double runTime
)
    : sineCoefficients(sineCoefficients),
      cosineCoefficients(cosineCoefficients),
      fundamentalFrequency(fundamentalFrequency),
      timeStep(timeStep),
      runTime(runTime)
{}


void FourierSeries::generate()
{
    // Remove any results from a previous call to generate().
    xValues.clear();
    yValues.clear();

    // Generate one waveform sample for each timestep.
    for (double time = 0.0;
         time <= runTime;
         time += timeStep) {

        double amplitude = 0.0;

        // Each coefficient corresponds to one harmonic.
        // Vector index 0 is harmonic 1, index 1 is harmonic 2, etc.
        for (std::size_t i = 0;
             i < sineCoefficients.size();
             ++i) {

            const int harmonic = static_cast<int>(i) + 1;

            // Calculate the angle 2*pi*n*f*t for this harmonic.
            const double angle =
                2.0
                * std::acos(-1.0)
                * harmonic
                * fundamentalFrequency
                * time;

            // A cosine coefficient may not exist if the cosine vector
            // is shorter than the sine coefficient vector.
            if (i < cosineCoefficients.size()) {
                amplitude +=
                    cosineCoefficients[i] * std::cos(angle);
            }

            amplitude +=
                sineCoefficients[i] * std::sin(angle);
        }

        xValues.push_back(time);
        yValues.push_back(amplitude);
    }
}


const std::vector<double>& FourierSeries::getXValues() const
{
    return xValues;
}


const std::vector<double>& FourierSeries::getYValues() const
{
    return yValues;
}