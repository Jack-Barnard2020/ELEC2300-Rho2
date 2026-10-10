/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: FourierSeries.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for Fourier series functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */
   
#include "FourierSeries.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

FourierSeries::FourierSeries(
    std::vector<double> sineCoefficients,
    std::vector<double> cosineCoefficients,
    double fundamentalFrequency,
    double timeStep,
    double runTime
)
    : sineCoefficients(std::move(sineCoefficients)),
      cosineCoefficients(std::move(cosineCoefficients)),
      fundamentalFrequency(fundamentalFrequency),
      timeStep(timeStep),
      runTime(runTime) {
    if (fundamentalFrequency < 0.0) throw std::invalid_argument("Fundamental frequency must be non-negative.");
    if (timeStep <= 0.0) throw std::invalid_argument("Time step must be greater than zero.");
    if (runTime < 0.0) throw std::invalid_argument("Run time must be non-negative.");
}

void FourierSeries::generate() {
    xValues.clear();
    yValues.clear();
    const std::size_t fullSteps = static_cast<std::size_t>(std::floor(runTime / timeStep));
    const bool needsFinalSample = fullSteps * timeStep < runTime;
    const std::size_t sampleCount = fullSteps + 1 + (needsFinalSample ? 1 : 0);
    const std::size_t harmonics = std::max(sineCoefficients.size(), cosineCoefficients.size());

    for (std::size_t sample = 0; sample < sampleCount; ++sample) {
        const double time = (sample == sampleCount - 1 && needsFinalSample) ? runTime : sample * timeStep;
        double amplitude = 0.0;
        for (std::size_t i = 0; i < harmonics; ++i) {
            const double angle = 2.0 * std::acos(-1.0) * (i + 1) * fundamentalFrequency * time;
            if (i < cosineCoefficients.size()) amplitude += cosineCoefficients[i] * std::cos(angle);
            if (i < sineCoefficients.size()) amplitude += sineCoefficients[i] * std::sin(angle);
        }
        xValues.push_back(time);
        yValues.push_back(amplitude);
    }
}

const std::vector<double>& FourierSeries::getXValues() const { return xValues; }
const std::vector<double>& FourierSeries::getYValues() const { return yValues; }
