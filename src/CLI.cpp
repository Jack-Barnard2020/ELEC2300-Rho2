/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: CLI.cpp
    Author: Jack Barnard
    Date: 2026/10/12
    Description: Implementation file for the command-line interface.
    Change Log:
        2026/10/12 - Initial commit (Jack Barnard)
   ====================================== */

#include "CLI.hpp"
#include "ReadNetlist.hpp"
#include "VoltageSource.hpp"
#include "NodalAnalysis.hpp"
#include "FirstOrderODESolver.hpp"
#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Plotter.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

namespace {

constexpr const char* RESET  = "\033[0m";
constexpr const char* BOLD   = "\033[1m";
constexpr const char* CYAN   = "\033[36m";
constexpr const char* GREEN  = "\033[32m";
constexpr const char* RED    = "\033[31m";
constexpr const char* YELLOW = "\033[33m";

} // namespace


void CLI::run()
{
    showWelcome();
    waitForEnter();

    while (running) {
        showMainMenu();

        std::string choice;

        std::cout << "\nSelect an option: ";
        std::getline(std::cin, choice);

        if (choice == "1") {
            loadNetlist();
        }
        else if (choice == "2") {
            running = false;
        }
        else {
            std::cout
                << RED
                << "\nInvalid option. Please enter 1 or 2.\n"
                << RESET;

            waitForEnter();
        }
    }

    showHeader("Goodbye");

    std::cout
        << "Thank you for using the circuit simulator.\n\n";
}


void CLI::clearScreen() const
{
    // Clear the terminal and move the cursor back to the top-left.
    std::cout << "\033[2J\033[H" << std::flush;
}


void CLI::showHeader(const std::string& pageTitle) const
{
    clearScreen();

    std::cout
        << BOLD
        << CYAN
        << "============================================================\n"
        << "                    CIRCUIT SIMULATOR\n"
        << "============================================================\n"
        << RESET;

    if (!pageTitle.empty()) {
        std::cout
            << '\n'
            << BOLD
            << pageTitle
            << RESET
            << '\n'
            << "------------------------------------------------------------\n";
    }

    if (!currentNetlistFilename.empty()) {
        std::cout
            << "Current netlist: "
            << GREEN
            << currentNetlistFilename
            << RESET
            << '\n'
            << "------------------------------------------------------------\n";
    }

    std::cout << '\n';
}


void CLI::showWelcome() const
{
    showHeader("Welcome");

    std::cout
        << "This program loads an electrical circuit from a netlist\n"
        << "and provides command-line circuit analysis.\n";
}


void CLI::showMainMenu() const
{
    showHeader("Main Menu");

    std::cout
        << "1. Load a netlist\n"
        << "2. Quit\n";
}


std::string CLI::promptForNetlist() const
{
    showHeader("Load Netlist");

    std::cout
        << "Enter the netlist filename or path.\n"
        << "Enter Q to cancel and return to the main menu.\n\n"
        << "> ";

    std::string filename;
    std::getline(std::cin, filename);

    return filename;
}


void CLI::loadNetlist()
{
    const std::string filename = promptForNetlist();

    if (filename == "q" || filename == "Q") {
        return;
    }

    if (filename.empty()) {
        std::cout
            << RED
            << "\nNo filename entered.\n"
            << RESET;

        waitForEnter();
        return;
    }

    showHeader("Loading Netlist");

    std::cout
        << "Reading "
        << filename
        << "...\n"
        << std::flush;

    try {
        // Parse the netlist and construct a new circuit.
        ReadNetlist reader(filename);
        Circuit loadedCircuit = reader.read();

        /*
         * Only replace the currently loaded circuit once the new
         * netlist has been read successfully.
         */
        circuit = std::move(loadedCircuit);
        currentNetlistFilename = filename;

        showComponentList();
        analysisMenu();
    }
    catch (const std::exception& error) {
        showHeader("Error Loading Netlist");

        std::cout
            << RED
            << "The netlist could not be loaded.\n\n"
            << RESET
            << error.what()
            << '\n';

        waitForEnter();
    }
}


void CLI::showComponentList() const
{
    showHeader("Component List");

    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    const auto& components = circuit->getComponents();

    std::cout
        << GREEN
        << "Netlist loaded successfully.\n"
        << RESET
        << '\n';

    // Table headings.
    std::cout
        << std::left
        << std::setw(5)  << "No."
        << std::setw(12) << "Name"
        << std::setw(16) << "Type"
        << std::setw(8)  << "Node 1"
        << std::setw(8)  << "Node 2"
        << std::setw(14) << "Value"
        << std::setw(16) << "Frequency"
        << "Phase\n";

    std::cout
        << "---------------------------------------------------------------------------------------\n";

    for (std::size_t index = 0;
         index < components.size();
         ++index) {

        const Component& component = *components.at(index);

        std::cout
            << std::left
            << std::setw(5)  << index + 1
            << std::setw(12) << component.getName()
            << std::setw(16) << component.getType()
            << std::setw(8)  << component.getNode1()
            << std::setw(8)  << component.getNode2()
            << std::setw(14) << component.getValue();

        /*
         * Voltage sources may contain additional waveform
         * information which is not part of the base Component class.
         */
        const VoltageSource* voltageSource =
            dynamic_cast<const VoltageSource*>(&component);

        if (voltageSource != nullptr) {

            /*
             * Replace these getter names if your VoltageSource.hpp
             * uses different names.
             */
            if (voltageSource->getFrequency() != 0.0) {
                std::cout
                    << std::setw(16)
                    << (std::to_string(voltageSource->getFrequency()) + " Hz")
                    << voltageSource->getPhase()
                    << " deg";
            }
            else {
                // DC voltage source.
                std::cout
                    << std::setw(16) << "-"
                    << "-";
            }
        }
        else {
            // R, L and C components have no source frequency/phase.
            std::cout
                << std::setw(16) << "-"
                << "-";
        }

        std::cout << '\n';
    }

    std::cout
        << "---------------------------------------------------------------------------------------\n"
        << "Number of components: "
        << circuit->getNumberOfComponents()
        << '\n'
        << "Highest node: "
        << circuit->getHighestNode()
        << '\n';

    waitForEnter();
}


void CLI::waitForEnter() const
{
    std::cout
        << YELLOW
        << "\nPress Enter to continue..."
        << RESET;

    std::cin.get();
}

void CLI::analysisMenu()
{
    bool analysisRunning = true;

    while (analysisRunning && running) {
        showHeader("Analysis Menu");

        std::cout
            << "Choose the type of analysis to perform:\n\n"
            << "1. DC Operating Point (Nodal Analysis)\n"
            << "2. Transient Analysis\n"
            << "3. Return to Main Menu\n"
            << "4. Quit\n\n"
            << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            runDCAnalysis();
        }
        else if (choice == "2") {
            runTransientAnalysis();
        }
        else if (choice == "3") {
            analysisRunning = false;
        }
        else if (choice == "4") {
            running = false;
            analysisRunning = false;
        }
        else {
            std::cout
                << RED
                << "\nInvalid option. Please enter 1, 2, 3 or 4.\n"
                << RESET;

            waitForEnter();
        }
    }
}

void CLI::runDCAnalysis() {
    showHeader("DC Operating Point");

    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    std::cout
        << "Calculating DC operating point...\n\n"
        << std::flush;

    try {
        NodalAnalysis analysis(*circuit);

        const auto nodeVoltages = analysis.solve();

        showHeader("DC Operating Point");

        std::cout
            << GREEN
            << "DC analysis completed successfully.\n"
            << RESET
            << '\n';

        std::cout
            << std::left
            << std::setw(12) << "Node"
            << "Voltage (V)\n";

        std::cout
            << "----------------------------\n";

        // Ground is always 0 V.
        std::cout
            << std::left
            << std::setw(12) << 0
            << 0.0
            << '\n';

        /*
         * The returned vector contains the voltages for
         * non-ground nodes.
         */
        for (std::size_t index = 0;
             index < nodeVoltages.size();
             ++index) {

            const std::size_t nodeNumber = index + 1;

            std::cout
                << std::left
                << std::setw(12) << nodeNumber
                << nodeVoltages.at(index)
                << '\n';
        }

        std::cout
            << "----------------------------\n";
    }
    catch (const std::exception& error) {
        std::cout
            << RED
            << "DC analysis failed.\n\n"
            << RESET
            << error.what()
            << '\n';
    }

    waitForEnter();
}

void CLI::runTransientAnalysis()
{
    showHeader("Transient Analysis");

    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // -----------------------------------------------------
    // Find the components required for a simple RC circuit.
    // -----------------------------------------------------

    const Resistor* resistor = nullptr;
    const Capacitor* capacitor = nullptr;
    const VoltageSource* source = nullptr;

    int resistorCount = 0;
    int capacitorCount = 0;
    int dcSourceCount = 0;

    for (const auto& componentPtr : circuit->getComponents()) {
        const Component* component = componentPtr.get();

        if (const auto* r =
                dynamic_cast<const Resistor*>(component)) {

            resistor = r;
            ++resistorCount;
        }
        else if (const auto* c =
                     dynamic_cast<const Capacitor*>(component)) {

            capacitor = c;
            ++capacitorCount;
        }
        else if (const auto* v =
                     dynamic_cast<const VoltageSource*>(component)) {

            // For now a frequency of zero is treated as a DC source.
            if (v->getFrequency() == 0.0) {
                source = v;
                ++dcSourceCount;
            }
        }
    }

    // -----------------------------------------------------
    // Validate the circuit.
    // -----------------------------------------------------

    if (resistorCount != 1 ||
        capacitorCount != 1 ||
        dcSourceCount != 1) {

        std::cout
            << RED
            << "Transient analysis currently supports a simple "
            << "first-order RC circuit.\n\n"
            << RESET
            << "The circuit must contain exactly:\n"
            << "  - one resistor\n"
            << "  - one capacitor\n"
            << "  - one DC voltage source\n\n"
            << "Detected:\n"
            << "  Resistors:          " << resistorCount << '\n'
            << "  Capacitors:         " << capacitorCount << '\n'
            << "  DC voltage sources: " << dcSourceCount << '\n';

        waitForEnter();
        return;
    }

    if (resistor == nullptr ||
        capacitor == nullptr ||
        source == nullptr) {

        std::cout
            << RED
            << "The RC components could not be identified.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // -----------------------------------------------------
    // Extract circuit parameters.
    // -----------------------------------------------------

    const double R = resistor->getValue();
    const double C = capacitor->getValue();
    const double Vs = source->getValue();

    if (R <= 0.0 || C <= 0.0) {
        std::cout
            << RED
            << "Resistance and capacitance must be greater than zero.\n"
            << RESET;

        waitForEnter();
        return;
    }

    const double timeConstant = R * C;

    std::cout
        << GREEN
        << "Simple RC circuit detected.\n"
        << RESET
        << '\n'
        << "Resistance:     " << R << " ohms\n"
        << "Capacitance:    " << C << " F\n"
        << "Source voltage: " << Vs << " V\n"
        << "Time constant:  " << timeConstant << " s\n\n";

    // -----------------------------------------------------
    // Ask the user for simulation parameters.
    // -----------------------------------------------------

    double initialVoltage = 0.0;
    double stopTime = 0.0;
    double timeStep = 0.0;

    std::cout << "Initial capacitor voltage (V): ";

    if (!(std::cin >> initialVoltage)) {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << RED
            << "\nInvalid initial voltage.\n"
            << RESET;

        waitForEnter();
        return;
    }

    std::cout << "Simulation stop time (s): ";

    if (!(std::cin >> stopTime)) {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << RED
            << "\nInvalid stop time.\n"
            << RESET;

        waitForEnter();
        return;
    }

    std::cout << "Simulation time step (s): ";

    if (!(std::cin >> timeStep)) {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << RED
            << "\nInvalid time step.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // Remove the newline left by operator >>.
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    // -----------------------------------------------------
    // Validate simulation parameters.
    // -----------------------------------------------------

    if (stopTime <= 0.0) {
        std::cout
            << RED
            << "\nStop time must be greater than zero.\n"
            << RESET;

        waitForEnter();
        return;
    }

    if (timeStep <= 0.0) {
        std::cout
            << RED
            << "\nTime step must be greater than zero.\n"
            << RESET;

        waitForEnter();
        return;
    }

    if (timeStep > stopTime) {
        std::cout
            << RED
            << "\nTime step cannot be greater than stop time.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // -----------------------------------------------------
    // Define the first-order RC differential equation.
    //
    //              Vs - Vc
    //     dVc/dt = ---------
    //                 R C
    //
    // The ODE solver interprets:
    //
    //     x = time
    //     y = capacitor voltage
    //
    // -----------------------------------------------------

    auto rcEquation =
        [R, C, Vs](double time, double capacitorVoltage) {
        return (Vs - capacitorVoltage) / (R * C);
    };

    // -----------------------------------------------------
    // Run the existing first-order ODE solver.
    // -----------------------------------------------------

    try {
        showHeader("Running Transient Analysis");

        std::cout
            << "Solving RC differential equation...\n\n"
            << "Initial voltage: " << initialVoltage << " V\n"
            << "Stop time:       " << stopTime << " s\n"
            << "Time step:       " << timeStep << " s\n\n"
            << std::flush;

        FirstOrderODESolver solver;

        solver.solveEuler(
            rcEquation,
            0.0,               // Initial time
            initialVoltage,    // Initial capacitor voltage
            timeStep,          // Euler step
            stopTime           // Final time
        );

        // -------------------------------------------------
        // Retrieve the solver output.
        // -------------------------------------------------

        const auto& timeValues = solver.getXValues();
        const auto& voltageValues = solver.getYValues();

        if (timeValues.empty() || voltageValues.empty()) {
            throw std::runtime_error(
                "The ODE solver returned no simulation data."
            );
        }

        if (timeValues.size() != voltageValues.size()) {
            throw std::runtime_error(
                "The transient X and Y vectors have different sizes."
            );
        }

        // -------------------------------------------------
        // Create the plot data series.
        // -------------------------------------------------

        Series capacitorSeries;

        capacitorSeries.name = "Capacitor Voltage";
        capacitorSeries.y = voltageValues;
        capacitorSeries.symbol = '*';
        capacitorSeries.color = PlotColor::Cyan;

        std::vector<Series> series = {
            capacitorSeries
        };

        // -------------------------------------------------
        // Create the terminal plot.
        // -------------------------------------------------

        Plot plot(
            60,     // Plot width
            "s",    // X-axis units
            "V"     // Y-axis units
        );

        // -------------------------------------------------
        // Display the result.
        // -------------------------------------------------

        showHeader("Transient Analysis Results");

        std::cout
            << GREEN
            << "Transient analysis completed successfully.\n"
            << RESET
            << '\n'
            << "Circuit parameters\n"
            << "------------------------------------------------------------\n"
            << "R:                 " << R << " ohms\n"
            << "C:                 " << C << " F\n"
            << "Source voltage:    " << Vs << " V\n"
            << "Time constant:     " << timeConstant << " s\n"
            << "Initial voltage:   " << initialVoltage << " V\n"
            << "Simulation length: " << stopTime << " s\n"
            << "Time step:         " << timeStep << " s\n"
            << "Calculated points: " << timeValues.size() << "\n\n";

        std::cout << plot.plot(
            timeValues,
            series
        );
    }
    catch (const std::exception& error) {
        showHeader("Transient Analysis Error");

        std::cout
            << RED
            << "Transient analysis failed.\n\n"
            << RESET
            << error.what()
            << '\n';
    }

    waitForEnter();
}