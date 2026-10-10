# ELEC2300 Rho2 Circuit Simulator

This repository contains supporting components for an ELEC2300 circuit
simulator.

The current version provides:

- A terminal-based plotting class
- Support for plotting one or more data series
- Optional ANSI terminal colours
- A Fourier-series waveform generator
- Example programmes for checking the plotting functionality

The circuit simulation functionality will be added in a later version of the
project.

## Project structure

```text
.
├── Plotter.hpp
├── Plotter.cpp
├── FourierSeries.hpp
├── FourierSeries.cpp
├── tests/
│   ├── test_linear.cpp
│   ├── test_square_wave.cpp
│   └── test_multiple_plots.cpp
├── Makefile
└── README.md
```

## Requirements

The project requires:

- A compiler supporting C++17
- GNU Make
- A terminal that supports ANSI colour escape sequences

For example, on Linux:

```bash
sudo apt install g++ make
```

The programme can still run in a terminal without colour support, although
ANSI escape sequences may be displayed incorrectly.

## Building the project

Build all test programmes with:

```bash
make
```

The executables are placed in the `build` directory:

```text
build/test_linear
build/test_square_wave
build/test_multiple_plots
```

## Running the tests

Run all tests in sequence with:

```bash
make test
```

Alternatively, run each test separately.

### Linear plot

```bash
./build/test_linear
```

This plots the function:

```text
y = x
```

### Square-wave approximation

```bash
./build/test_square_wave
```

This uses a Fourier series to approximate a square wave. Increasing the number
of odd harmonics improves the approximation, but also increases the amount of
calculation.

### Multiple plots

```bash
./build/test_multiple_plots
```

This plots sine and cosine waveforms on the same terminal graph.

## Cleaning generated files

Remove the `build` directory and all compiled files with:

```bash
make clean
```

## Using the Plot class

Include the plotting header:

```cpp
#include "Plotter.hpp"
```

Create the X values and a data series:

```cpp
std::vector<double> x = {
    0.0,
    1.0,
    2.0,
    3.0
};

Series line = {
    {0.0, 1.0, 2.0, 3.0},
    "y = x",
    '*',
    PlotColor::Green
};
```

Create a plot by supplying:

1. The terminal plot width
2. The X-axis units
3. The Y-axis units

```cpp
Plot plotter(60, "s", "V");
```

Generate and print the plot:

```cpp
std::cout << plotter.plot(x, {line});
```

The `plot()` function returns the completed plot as a `std::string`. This
allows the caller to print it, save it, or include it in another output
interface.

## Plot colours

The following colours are supported:

```cpp
PlotColor::Default
PlotColor::Red
PlotColor::Green
PlotColor::Yellow
PlotColor::Blue
PlotColor::Magenta
PlotColor::Cyan
PlotColor::White
```

Example:

```cpp
Series voltage = {
    voltageValues,
    "Voltage",
    'V',
    PlotColor::Cyan
};
```

Use `PlotColor::Default` if terminal colouring is not required.

## Plotting multiple series

Create multiple `Series` objects and pass them in the same vector:

```cpp
Series voltage = {
    voltageValues,
    "Voltage",
    'V',
    PlotColor::Green
};

Series current = {
    currentValues,
    "Current",
    'I',
    PlotColor::Yellow
};

std::cout << plotter.plot(
    timeValues,
    {voltage, current}
);
```

Every series must contain exactly one Y value for each X value.

If two series occupy the same terminal position, the series processed last is
displayed at that position.

## Using the FourierSeries class

Include the Fourier-series header:

```cpp
#include "FourierSeries.hpp"
```

Create the coefficient vectors:

```cpp
std::vector<double> sineCoefficients = {
    1.0,
    0.0,
    1.0 / 3.0
};

std::vector<double> cosineCoefficients = {
    0.0,
    0.0,
    0.0
};
```

Construct the generator:

```cpp
FourierSeries waveform(
    sineCoefficients,
    cosineCoefficients,
    1.0,
    0.01,
    2.0
);
```

The constructor arguments are:

1. Sine coefficients
2. Cosine coefficients
3. Fundamental frequency
4. Simulation time step
5. Total run time

Generate the waveform:

```cpp
waveform.generate();
```

Access the generated values:

```cpp
const std::vector<double>& time =
    waveform.getXValues();

const std::vector<double>& values =
    waveform.getYValues();
```

The generated vectors can then be passed to the plotting class.

## Fourier coefficient indexing

Element zero of each coefficient vector represents the first harmonic.

For example:

```cpp
sineCoefficients[0]
```

is the coefficient of:

```text
sin(2 pi f t)
```

and:

```cpp
sineCoefficients[2]
```

is the coefficient of:

```text
sin(2 pi 3 f t)
```

Zero coefficients must therefore be included when a harmonic is omitted.

For example, the following vector uses the first, third, and fifth harmonics:

```cpp
std::vector<double> coefficients = {
    1.0,
    0.0,
    1.0 / 3.0,
    0.0,
    1.0 / 5.0
};
```

## Current limitations

The current version has the following limitations:

- The plot is horizontal, with one terminal row per X value.
- Plot positions are scaled using the minimum and maximum Y values across all
  supplied series.
- Overlapping symbols are not combined.
- ANSI colours may not work in every terminal.
- The Fourier-series generator does not currently include a DC coefficient.
- The simulator itself has not yet been added.
- Invalid plotting input is reported to `std::cerr`, and `plot()` returns an
  empty string.
- Fourier-series constructor parameters are not currently validated.

## Planned development

The README will be updated when the circuit simulator is integrated. Possible
future additions include:

- Circuit input parsing
- Component models
- Circuit validation
- Simulation control
- Voltage and current waveform generation
- Improved plotting axes and labels
- Automated unit tests
- File export

## Author

Jack Barnard

ELEC2300 Rho2