#include "FourierSeries.hpp"
#include "Plotter.hpp"

#include <vector>

int main()
{
	FourierSeries sineWave(
		{1.3, 0.0, 0.4, 0.0, 0.3, 0.0, 0.2, 0.0, 0.1}, // Sine coefficients
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // Cosine coefficients
		1.0,
		0.01,
		2.0
	);
	sineWave.generate();

	Plot plotter(80, "s", "amplitude");
	Series series{
		sineWave.getYValues(),
		"Fourier sine wave",
		'*'
	};
	plotter.plot(sineWave.getXValues(), {series});

	return 0;
}
