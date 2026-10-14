# Rho2 Circuit Simulator

A command-line circuit simulator written in C++ for the ELEC2300 Rho2 project.

The program reads a circuit from a netlist, displays the loaded components, and provides analysis tools from an interactive terminal menu.

## Using the simulator

1. Run the compiled circuit simulator from a terminal.
2. At the main menu, select the option to load a netlist.
3. Enter the path or filename of the netlist when prompted.
4. Check the component list to make sure the circuit has been read correctly.
5. Open the analysis menu and select the required analysis.
6. Follow the prompts for any additional simulation values.
7. Return to the menu to run another analysis or quit the program.

The interface is menu driven, so the available commands and required values are shown in the terminal as they are needed.

## Circuit netlists

Circuit descriptions are supplied to the simulator using a text netlist. Each component entry describes the component name, the nodes it is connected between, and the component value or source parameters.

Use node `0` as the ground/reference node.

A simple RC circuit, for example, should contain:

- one voltage source;
- one resistor;
- one capacitor;
- a ground/reference connection at node `0`.

Keep component names unique within the netlist. If the simulator rejects a netlist, check the component names, node numbers, values and source definition before running the analysis again.

## Analysis modes

### DC operating point

The DC analysis calculates the circuit's node voltages using nodal analysis.

Use this mode when you want the steady-state DC operating point of the loaded circuit.

### Transient analysis

Transient analysis simulates how a simple first-order RC circuit changes with time. The simulator uses a first-order ODE solver and displays the calculated response as a terminal graph.

For an RC charging simulation, you will be asked for simulation settings such as the initial capacitor voltage, stop time and time step.

A smaller time step normally gives a finer representation of the response, but also creates more simulation points.

### Bang-bang controller simulation

The bang-bang controller simulation adds feedback to a simple RC transient simulation.

The controller has two output states:

- maximum output when the capacitor voltage is below the target voltage;
- minimum output when the capacitor voltage is at or above the target voltage.

For example, with a target of `3 V` and an output range of `0 V` to `5 V`, the supply switches between the two limits as the simulated capacitor voltage moves around the target.

For this mode, load a simple first-order RC circuit containing one resistor, one capacitor and one DC voltage source. The program then asks for the controller target and transient simulation settings.

The results plot shows the simulated capacitor voltage together with the target voltage so that the controller behaviour can be seen directly.

## Choosing a time step

The time step must be greater than zero and should be smaller than the total simulation time.

For RC simulations, compare the chosen time step with the circuit time constant:

```text
tau = R * C
```

If the graph looks very coarse or the switching behaviour is difficult to see, reduce the time step and run the simulation again.

## Example workflow

A typical controlled RC simulation is:

```text
Start simulator
    |
Load RC netlist
    |
Check component list
    |
Open Analysis Menu
    |
Select Bang-Bang Controller Simulation
    |
Enter target voltage
    |
Enter initial capacitor voltage
    |
Enter stop time and time step
    |
View plotted capacitor response
```

For a `5 V` source and `3 V` target, a test configuration could use:

```text
Target voltage:             3.0 V
Initial capacitor voltage:  0.0 V
```

Choose the stop time and time step according to the `R * C` time constant of the circuit you loaded.

## Troubleshooting

### Netlist will not load

Check that:

- the filename/path is correct;
- the file is a plain-text netlist;
- component names are unique;
- node numbers are valid;
- component values are valid numbers;
- the circuit includes node `0` where a ground/reference node is required.

### Transient simulation is rejected

The current transient/controller simulation is intended for a simple first-order RC circuit. Check that the circuit contains the components required by the selected analysis.

### Controller target is rejected

The target voltage must lie within the controller's minimum and maximum output range. With a `0 V` to `5 V` controller, for example, the target must lie within that range.

### Graph looks inaccurate or too coarse

Reduce the simulation time step. The Euler method advances the solution one discrete step at a time, so the chosen step size affects the numerical resolution of the plotted response.

### Terminal formatting looks wrong

The CLI uses terminal formatting for menus and results. Use a terminal that supports ANSI escape sequences if colour or screen clearing does not display correctly.

## Project structure

The simulator separates circuit representation, components, analysis and user-interface code into different classes. The main source files include implementations for components such as resistors, capacitors, inductors and voltage sources, alongside netlist reading, nodal analysis, ODE solving, plotting, the controller and the CLI.

This separation keeps the circuit model and numerical analysis independent from the command-line interface, making the simulator easier to extend with additional components and analysis methods.
