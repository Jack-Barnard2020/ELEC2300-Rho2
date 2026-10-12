# ELEC2300 Circuit Simulator

A C++ circuit simulation project developed for ELEC2300 / Rho 2.

The project provides a set of classes for representing electrical components and circuits, reading circuits from netlist files, performing DC nodal analysis, generating numerical data, and displaying results in the terminal.

## Overview

The simulator is organised around an object-oriented component model.

Electrical components share a common `Component` base class. Individual component types such as resistors, capacitors, inductors and voltage sources implement the behaviour specific to that component.

A `Circuit` object owns the components that form a circuit. Circuits can either be constructed programmatically or populated from a text netlist using `ReadNetlist`.

The current implementation contains support for:

- Resistors
- Capacitors
- Inductors
- DC voltage sources
- Sinusoidal AC voltage sources
- Netlist parsing
- Engineering suffixes such as `k`, `m`, `u`, `n` and `p`
- DC modified nodal analysis
- First-order ODE solving using Euler's method
- Fourier-series waveform generation
- Terminal-based plotting of one or more data series

---

## Project Structure

The implementation is split into a number of modules.

### `Component`

`Component` is the common base representation for a two-terminal circuit component.

Each component stores:

- a component name
- node 1
- node 2
- a numeric component value

The constructor performs common validation:

- component names cannot be empty
- node numbers cannot be negative
- the two terminals cannot connect to the same node

Node `0` represents ground.

### `Resistor`

Represents an ideal resistor.

The class provides:

- resistance
- conductance
- current for a supplied voltage
- component type information

The resistor follows Ohm's law:

    I = V / R

and conductance is calculated as:

    G = 1 / R

Resistance must be greater than zero.

### `Capacitor`

Represents an ideal capacitor.

The capacitor current can be calculated from the rate of change of voltage:

    I = C * dV/dt

Capacitance must be greater than zero.

For DC steady-state nodal analysis, capacitors are treated as open circuits.

### `Inductor`

Represents an ideal inductor.

The voltage across the inductor can be calculated using:

    V = L * dI/dt

Inductance must be greater than zero.

For DC steady-state nodal analysis, inductors are treated as short circuits.

### `VoltageSource`

Represents either a DC or sinusoidal AC voltage source.

Two forms are supported:

    VoltageSource(name, positiveNode, negativeNode, voltage)

and:

    VoltageSource(
        name,
        positiveNode,
        negativeNode,
        amplitude,
        frequency,
        phase
    )

A DC source returns a constant voltage.

An AC source is evaluated as:

    V(t) = amplitude * sin(2*pi*frequency*t + phase)

Frequency cannot be negative.

Phase is specified in radians.

AC voltage sources are not stamped into the current DC nodal-analysis system.

### `Circuit`

`Circuit` is the container for all components in a circuit.

Components are stored using `std::unique_ptr<Component>`, meaning ownership of a component is transferred to the circuit when it is added.

The class provides functionality to:

- add a component
- retrieve all components
- find a component by name
- obtain the number of components
- determine the highest numbered node

Component names must be unique within a circuit.

Attempting to add a null component or a second component with an existing name throws `std::invalid_argument`.

### `ReadNetlist`

`ReadNetlist` converts a text-based netlist into a `Circuit`.

It:

1. opens the requested file
2. reads it one line at a time
3. removes comments
4. tokenises each remaining line
5. validates the fields
6. creates the appropriate component
7. adds the component to the circuit

Blank lines and comment-only lines are ignored.

Comments begin with:

    #

### `NodalAnalysis`

`NodalAnalysis` performs DC modified nodal analysis on a `Circuit`.

The solver:

1. creates the system matrix
2. stamps resistor conductances
3. adds DC voltage sources
4. treats capacitors as DC open circuits
5. treats inductors as DC short circuits
6. solves the resulting linear system
7. returns the calculated non-ground node voltages

The linear system is solved using Gaussian elimination with pivot selection.

If the matrix is singular, for example because of a floating node or incompatible ideal-source configuration, the solver throws a runtime error.

Ground node `0` is not included in the returned node-voltage vector.

### `FirstOrderODESolver`

Provides a first-order numerical ODE solver using Euler's method.

`solveEuler()` accepts:

- a derivative function
- initial X value
- initial Y value
- step size
- final X value

The resulting X and Y samples can then be retrieved using the corresponding getter functions.

The solver validates that:

- the derivative function is valid
- the step size is greater than zero
- the final X value is not less than the initial X value

If the requested interval is not an exact multiple of the step size, the final step is reduced so that the final sample occurs at `finalX`.

### `FourierSeries`

Generates a sampled waveform from sine and cosine Fourier coefficients.

The class stores:

- sine coefficients
- cosine coefficients
- fundamental frequency
- sample time step
- total run time

Each harmonic uses a multiple of the fundamental frequency.

Calling `generate()` replaces any previously generated samples.

The generated samples are available through `getXValues()` and `getYValues()`.

### `Plotter`

Provides terminal-based visualisation of numerical data.

A plot is created from:

- an X vector
- one or more data series
- X-axis units
- Y-axis units
- a terminal plot width

Multiple series can be displayed using different symbols and ANSI terminal colours.

All plotted Y series must contain the same number of samples as the X vector.

---

## Netlist Format

A circuit can be described using a plain-text netlist.

Each component occupies one line.

The general passive/DC format is:

    name node1 node2 type value

Supported component identifiers are:

| Identifier | Component |
|------------|-----------|
| `R` | Resistor |
| `C` | Capacitor |
| `L` | Inductor |
| `VDC` | DC voltage source |
| `VAC` | AC voltage source |

### Examples

A resistor:

    R1 1 0 R 1k

A capacitor:

    C1 1 0 C 100u

An inductor:

    L1 1 2 L 10m

A DC voltage source:

    V1 1 0 VDC 5

### AC voltage source

AC voltage sources require two additional parameters:

    name node1 node2 VAC amplitude frequency phase

For example:

    V1 1 0 VAC 5 50 0

This defines the parameters required by the sinusoidal voltage-source model.

---

## Engineering Suffixes

The netlist parser supports engineering notation.

| Suffix | Multiplier |
|--------|------------|
| `T` | 1e12 |
| `G` | 1e9 |
| `M` | 1e6 |
| `k` | 1e3 |
| `m` | 1e-3 |
| `u` | 1e-6 |
| `n` | 1e-9 |
| `p` | 1e-12 |

For example:

    10k
    47m
    100u
    220n

Values without a suffix are also accepted.

Suffixes are case-sensitive.

---

## Example Netlist

An example resistive DC circuit could be written as:

    # Example voltage-divider circuit

    V1 1 0 VDC 10
    R1 1 2 R 1k
    R2 2 0 R 1k

Node `0` is ground.

The remaining node numbers correspond to unknown node voltages calculated by `NodalAnalysis`.

---

## Using the Classes

### Reading a circuit

A circuit can be created from a netlist using `ReadNetlist`:

    ReadNetlist reader("circuit.txt");
    Circuit circuit = reader.read();

If the file cannot be opened, contains invalid data, or does not contain any components, an exception is thrown.

### Building a circuit directly

Components can also be constructed in C++ and added to a circuit.

For example:

    Circuit circuit;

    circuit.addComponent(
        std::make_unique<VoltageSource>("V1", 1, 0, 10.0)
    );

    circuit.addComponent(
        std::make_unique<Resistor>("R1", 1, 2, 1000.0)
    );

    circuit.addComponent(
        std::make_unique<Resistor>("R2", 2, 0, 1000.0)
    );

Because `Circuit` owns its components through `std::unique_ptr`, ownership is transferred when the component is added.

### Running DC analysis

Given a populated `Circuit`:

    NodalAnalysis analysis(circuit);

    std::vector<double> nodeVoltages = analysis.solve();

The returned vector contains voltages for the non-ground nodes.

Because node `0` is excluded, the result at vector index `0` corresponds to circuit node `1`, index `1` corresponds to circuit node `2`, and so on.

---

## Numerical Utilities

The circuit classes are accompanied by general numerical utilities that can be used for waveform and transient-related calculations.

### Euler ODE solver

A first-order differential equation can be numerically sampled with `FirstOrderODESolver`.

Conceptually:

    dy/dx = f(x, y)

is advanced using Euler's method:

    y_next = y + h * f(x, y)

where `h` is the step size.

After solving, use:

    getXValues()
    getYValues()

to access the generated samples.

### Fourier series

`FourierSeries` generates time-domain values by combining sine and cosine harmonics.

After constructing the series:

    series.generate();

the generated samples are available from:

    series.getXValues();
    series.getYValues();

---

## Plotting Data

`Plotter` converts numerical sample vectors into a text representation suitable for displaying in a terminal.

Each `Series` contains its:

- name
- Y values
- plotting symbol
- colour

All series share the same X axis.

The plotter automatically determines the minimum and maximum Y values across the supplied data and maps those values onto the requested terminal width.

It also displays:

- a legend
- X values
- the graphical row
- numeric Y values
- the overall Y range

ANSI escape sequences are used for coloured output.

---

## Error Handling

The project uses standard C++ exceptions for invalid circuit definitions and invalid numerical parameters.

Examples of invalid input include:

- empty component names
- negative node numbers
- connecting both terminals of a component to the same node
- zero or negative resistance
- zero or negative capacitance
- zero or negative inductance
- negative AC frequency
- duplicate component names
- null components
- malformed netlist lines
- unknown netlist component types
- invalid engineering suffixes
- empty netlists
- invalid ODE solver parameters
- singular DC-analysis matrices

Code using the simulator should therefore handle exceptions where appropriate, for example:

    try {
        ReadNetlist reader("circuit.txt");
        Circuit circuit = reader.read();

        NodalAnalysis analysis(circuit);
        const auto voltages = analysis.solve();

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
    }

---

## Building the Project

The implementation files expect corresponding header files such as:

    Component.hpp
    Resistor.hpp
    Capacitor.hpp
    Inductor.hpp
    VoltageSource.hpp
    Circuit.hpp
    ReadNetlist.hpp
    NodalAnalysis.hpp
    FirstOrderODESolver.hpp
    FourierSeries.hpp
    Plotter.hpp

The source files should be compiled together with the program entry point (`main.cpp` or equivalent).

A compiler supporting the C++ standard required by the project should be used.

`FourierSeries.cpp` uses `std::numbers::pi`, so the project requires C++20 support unless that implementation is changed.

For example, with GCC and all required `.cpp` files in the same directory:

    g++ -std=c++20 *.cpp -o circuit_simulator

The executable can then be run from the terminal.

On Linux/macOS:

    ./circuit_simulator

The exact command-line arguments or interactive usage depend on the project's `main.cpp`, which is not documented here because it is not part of the current source set.

---

## Current DC Analysis Behaviour

It is important to distinguish the capabilities of the component classes from the current DC solver.

During `NodalAnalysis`:

- resistors are stamped using their conductance
- DC voltage sources are included using additional modified-nodal-analysis unknowns
- capacitors are treated as open circuits
- inductors are treated as short circuits
- AC voltage sources are skipped during DC analysis

This means the presence of AC-source and time-domain utility classes does not mean that `NodalAnalysis::solve()` performs a full transient or AC-frequency-domain simulation.

---

## Design Notes

The project separates responsibilities between classes:

- `Component` stores information common to circuit elements.
- `Resistor`, `Capacitor`, `Inductor`, and `VoltageSource` implement component-specific behaviour.
- `Circuit` manages component ownership and circuit topology metadata.
- `ReadNetlist` handles file parsing.
- `NodalAnalysis` handles DC circuit solving.
- `FirstOrderODESolver` provides numerical integration support.
- `FourierSeries` generates sampled periodic signals.
- `Plotter` handles terminal presentation.

This structure keeps electrical modelling, parsing, calculation, storage and output concerns separate, making the code easier to extend.

---

## Extending the Simulator

When adding another component type, a programmer should generally consider:

1. how the component is represented in the class hierarchy
2. what constructor validation it requires
3. whether `ReadNetlist` needs a new component identifier
4. how the component behaves under each supported type of analysis
5. whether new matrix-stamping logic is required
6. what tests are required for valid and invalid inputs

When extending the analysis functionality, take care not to assume that the existing DC behaviour is also correct for transient or AC analysis.

---

## Known Scope

The current source code explicitly implements DC nodal analysis.

Transient-related building blocks are present, including:

- capacitor and inductor differential relationships
- sinusoidal voltage generation
- Euler ODE integration
- Fourier-series generation
- plotting

However, the current `NodalAnalysis` implementation itself solves the DC system. A complete transient or AC circuit-analysis pipeline should not be assumed until it is explicitly implemented and tested.

---

## Author

Jack Barnard

ELEC2300 / Rho 2  
Circuit Simulator

Initial implementation: October 2026
