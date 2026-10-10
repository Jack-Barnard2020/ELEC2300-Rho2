/*
 * Test: multiple plotted series
 *
 * Checks that the Plot class can display sine and cosine waveforms using
 * different symbols and colours.
 */

#include "Plotter.hpp"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    const double pi = std::acos(-1.0);
    const double timeStep = 0.05;
    const double runTime = 2.0;
    const double frequency = 1.0;

    std::vector<double> timeValues;
    std::vector<double> sineValues;
    std::vector<double> cosineValues;

    /*
     * Add a small fraction of the time step to the loop limit to reduce
     * the chance of omitting the final point because of floating-point
     * rounding.
     */
    for (double time = 0.0;
         time <= runTime + timeStep / 2.0;
         time += timeStep) {
        const double angle =
            2.0 * pi * frequency * time;

        timeValues.push_back(time);
        sineValues.push_back(std::sin(angle));
        cosineValues.push_back(std::cos(angle));
    }

    const Series sineSeries{
        sineValues,
        "sin(2 pi t)",
        'S',
        PlotColor::Green
    };

    const Series cosineSeries{
        cosineValues,
        "cos(2 pi t)",
        'C',
        PlotColor::Magenta
    };

    const Plot plotter(
        60,
        "s",
        "V"
    );

    std::cout << "Multiple-series plot test\n\n";
    std::cout << plotter.plot(
        timeValues,
        {sineSeries, cosineSeries}
    );

    return 0;
}