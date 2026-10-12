#pragma once

#include <vector>

// Generates a waveform from matching sine and cosine harmonic coefficients.
class FourierSeries {
public:
    FourierSeries(
        std::vector<double> sineCoefficients,
        std::vector<double> cosineCoefficients,
        double fundamentalFrequency,
        double timeStep,
        double runTime
    );

    void generate();

    [[nodiscard]] const std::vector<double>& getXValues() const;
    [[nodiscard]] const std::vector<double>& getYValues() const;

private:
    std::vector<double> sineCoefficients;
    std::vector<double> cosineCoefficients;
    double fundamentalFrequency;
    double timeStep;
    double runTime;
    std::vector<double> xValues;
    std::vector<double> yValues;
};
