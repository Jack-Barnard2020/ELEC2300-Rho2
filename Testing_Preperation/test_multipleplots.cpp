#include "Plotter.hpp"

#include <iostream>
#include <vector>

int main()
{
	constexpr double minimum = -10.0;
	constexpr double maximum = 10.0;
	constexpr double step = 0.1;
	constexpr int numberOfPoints =
		static_cast<int>((maximum - minimum) / step) + 1;

	std::vector<double> x(numberOfPoints);
	std::vector<double> positiveY(numberOfPoints);
	std::vector<double> negativeY(numberOfPoints);

	for (int i = 0; i < numberOfPoints; ++i) {
		x[i] = minimum + step * i;
		positiveY[i] = x[i];
		negativeY[i] = -x[i];
	}

	Series positiveX = {
		positiveY,
		"Positive X",
		'+',
		PlotColor::Red
	};

	Series negativeX = {
		negativeY,
		"Negative X",
		'-',
		PlotColor::Blue
	};

	Plot plot(80, "x", "y");
	std::string generatedPlot = plot.plot(x, {positiveX, negativeX});
	std::cout << generatedPlot;
	return 0;
}
