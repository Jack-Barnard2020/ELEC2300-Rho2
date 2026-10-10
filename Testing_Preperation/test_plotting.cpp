#include "Plotter.hpp"

#include <iostream>
#include <vector>

int main()
{
    constexpr int numberOfPoints = 201;
    constexpr double minimum = -10.0;
    constexpr double maximum = 10.0;

    std::vector<double> x(numberOfPoints);
    std::vector<double> y(numberOfPoints);

    for (int i = 0; i < numberOfPoints; ++i) {
        x[i] = minimum
            + (maximum - minimum) * i / (numberOfPoints - 1);
        y[i] = x[i];
    }

    Plot plotter(80, "x", "y");
    Series series{y, "y = x", '*', PlotColor::Red};

    const std::string plotOutput = plotter.plot(x, {series});
    std::cout << plotOutput;

    return 0;
}