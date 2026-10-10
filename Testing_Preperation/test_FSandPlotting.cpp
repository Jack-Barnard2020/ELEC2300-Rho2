#include "FourierSeries.hpp"
#include "Plotter.hpp"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
	constexpr int numberOfCoefficients = 100;
	constexpr double pi = 3.14159265358979323846;
	std::vector<double> sineCoefficients;
	std::vector<double> cosineCoefficients(
		numberOfCoefficients,
		0.00000
	);

	sineCoefficients.reserve(numberOfCoefficients);
	for (int i = 0; i < numberOfCoefficients; ++i) {
		const int harmonic = i + 1;
		double coefficient = 0.0;

		if (harmonic % 2 != 0) {
			coefficient = 4.0 / (pi * harmonic);
		}

		sineCoefficients.push_back(
			std::round(coefficient * 100000.0) / 100000.0
		);
	}

	FourierSeries sineWave(
		sineCoefficients,
		cosineCoefficients,
		1.0,
		0.01,
		2.0
	);
	sineWave.generate();

	Plot plotter(80, "s", "amplitude");
	Series series{
		sineWave.getYValues(),
		"Fourier sine wave",
		'*',
		PlotColor::Red
	};
	const std::string plotOutput =
		plotter.plot(sineWave.getXValues(), {series});
	std::cout << plotOutput;

	return 0;
}
