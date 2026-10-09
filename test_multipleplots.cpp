// Test plotting the function y = x.
#include "Plotter.hpp"

#include <vector>

int main()
{
	constexpr int number_of_points = 201;
	constexpr double minimum = -10.0;
	constexpr double maximum = 10.0;

	std::vector<double> x(number_of_points);
	std::vector<double> y(number_of_points);
	std::vector<double> negativeY(number_of_points);

	for (int i = 0; i < number_of_points; ++i) {
		x[i] = minimum + (maximum - minimum) * i / (number_of_points - 1);
		y[i] = x[i];
		negativeY[i] = -x[i];
	}

	Plot plotter(80, "s", "V");
	Series positiveSeries{y, "y = x", '*'};
	Series negativeSeries{negativeY, "y = -x", 'o'};
	plotter.plot(x, {positiveSeries, negativeSeries});
}
