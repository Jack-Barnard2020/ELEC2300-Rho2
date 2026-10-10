/*
 * Test: y = x
 *
 * Checks that the Plot class can display a simple linear data series.
 */

#include "Plotter.hpp"

#include <iostream>
#include <vector>

int main()
{
    std::vector<double> xValues;
    std::vector<double> yValues;

    // Generate points from -5 to +5.
    for (int value = -5; value <= 5; ++value) {
        const double point = static_cast<double>(value);

        xValues.push_back(point);
        yValues.push_back(point);
    }

    const Series linearSeries{
        yValues,
        "y = x",
        '*',
        PlotColor::Green
    };

    const Plot plotter(
        60,
        "x",
        "y"
    );

    std::cout << "Linear plot test\n\n";
    std::cout << plotter.plot(xValues, {linearSeries});

    return 0;
}