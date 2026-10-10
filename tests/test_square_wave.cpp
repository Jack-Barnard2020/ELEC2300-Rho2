/*
 * Test: square-wave Fourier approximation
 *
 * Uses odd sine harmonics to approximate a square wave.
 */

#include "FourierSeries.hpp"
#include "Plotter.hpp"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    const double pi = std::acos(-1.0);
    const double scale = 4.0 / pi;

    /*
     * Each vector position represents one harmonic:
     *
     * index 0 -> first harmonic
     * index 1 -> second harmonic
     * index 2 -> third harmonic
     *
     * A square wave contains odd sine harmonics. The even harmonic
     * coefficients are therefore explicitly set to zero.
     */
    const std::vector<double> sineCoefficients{
        scale,
        0.0,
        scale / 3.0,
        0.0,
        scale / 5.0,
        0.0,
        scale / 7.0,
        0.0,
        scale / 9.0
    };

    const std::vector<double> cosineCoefficients(
        sineCoefficients.size(),
        0.0
    );

    const double fundamentalFrequency = 1.0;
    const double timeStep = 0.025;
    const double runTime = 2.0;

    FourierSeries squareWave(
        sineCoefficients,
        cosineCoefficients,
        fundamentalFrequency,
        timeStep,
        runTime
    );

    squareWave.generate();

    const Series squareSeries{
        squareWave.getYValues(),
        "Square-wave approximation",
        '#',
        PlotColor::Cyan
    };

    const Plot plotter(
        60,
        "s",
        "V"
    );

    std::cout << "Square-wave Fourier-series test\n\n";
    std::cout << plotter.plot(
        squareWave.getXValues(),
        {squareSeries}
    );

    return 0;
}