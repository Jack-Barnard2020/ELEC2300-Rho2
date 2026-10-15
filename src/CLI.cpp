/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: CLI.cpp
    Author: Jack Barnard
    Date: 2026/10/12
    Description: Implementation file for the command-line interface.
    Change Log:
        2026/10/12 - Initial commit (Jack Barnard)
        2026/10/13 - Added transient analysis functionality and plotting (Jack Barnard)
        2026/10/13 - Improved commenting (Jack Barnard)
        2026/10/14 - Finished explanatory comments and documented design choices (Jack Barnard)
        2026/10/15 - Added RC, RL and RLC transient analysis support (Jack Barnard)
   ====================================== */

#include "CLI.hpp"
#include "ReadNetlist.hpp"
#include "VoltageSource.hpp"
#include "NodalAnalysis.hpp"
#include "FirstOrderODESolver.hpp"
#include "SecondOrderODESolver.hpp"
#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"
#include "Plotter.hpp"
#include "Controller.hpp"

#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define SCREEN_WIDTH 100

namespace {

constexpr const char* RESET  = "\033[0m";
constexpr const char* BOLD   = "\033[1m";
constexpr const char* CYAN   = "\033[36m";
constexpr const char* GREEN  = "\033[32m";
constexpr const char* RED    = "\033[31m";
constexpr const char* YELLOW = "\033[33m";
constexpr const char* CLEAR_SCREEN = "\033[2J";
constexpr const char* CURSOR_HOME  = "\033[H";

}

void CLI::run() {
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

void CLI::clearScreen() const {
    std::cout << CLEAR_SCREEN << CURSOR_HOME << std::flush;
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

void CLI::showWelcome() const {
    showHeader("Welcome");

    std::cout
        << "This program loads an electrical circuit from a netlist\n"
        << "and provides command-line circuit analysis.\n";
}

void CLI::showMainMenu() const {
    showHeader("Main Menu");

    std::cout
        << "1. Load a netlist\n"
        << "2. Quit\n";
}

std::string CLI::promptForNetlist() const {
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
        ReadNetlist reader(filename);
        Circuit loadedCircuit = reader.read();

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

void CLI::showComponentList() const {
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

    for (std::size_t index = 0; index < components.size(); ++index) {
        const Component& component = *components.at(index);

        std::cout
            << std::left
            << std::setw(5)  << index + 1
            << std::setw(12) << component.getName()
            << std::setw(16) << component.getType()
            << std::setw(8)  << component.getNode1()
            << std::setw(8)  << component.getNode2()
            << std::setw(14) << component.getValue();

        const VoltageSource* voltageSource =
            dynamic_cast<const VoltageSource*>(&component);

        if (voltageSource != nullptr) {
            if (voltageSource->getFrequency() != 0.0) {
                std::cout
                    << std::setw(16)
                    << (std::to_string(voltageSource->getFrequency()) + " Hz")
                    << voltageSource->getPhase()
                    << " deg";
            }
            else {
                std::cout
                    << std::setw(16) << "-"
                    << "-";
            }
        }
        else {
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

void CLI::waitForEnter() const {
    std::cout
        << YELLOW
        << "\nPress Enter to continue..."
        << RESET;

    std::cin.get();
}

void CLI::analysisMenu() {
    bool analysisRunning = true;

    while (analysisRunning && running) {
        showHeader("Analysis Menu");

        std::cout
            << "Choose the type of analysis to perform:\n\n"
            << "1. DC Operating Point (Nodal Analysis)\n"
            << "2. Transient Analysis\n"
            << "3. Bang-Bang Controller Simulation\n"
            << "4. Return to Main Menu\n"
            << "5. Quit\n\n"
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
            runBangBangControllerSimulation();
        }
        else if (choice == "4") {
            analysisRunning = false;
        }
        else if (choice == "5") {
            running = false;
            analysisRunning = false;
        }
        else {
            std::cout
                << RED
                << "\nInvalid option. Please enter 1, 2, 3, 4 or 5.\n"
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

        std::cout
            << std::left
            << std::setw(12) << 0
            << 0.0
            << '\n';

        for (std::size_t index = 0; index < nodeVoltages.size(); ++index) {
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

void CLI::runBangBangControllerSimulation() {
    showHeader("Bang-Bang Controller Simulation");

    if (!circuit.has_value()) {
        std::cout << RED << "No circuit is currently loaded.\n" << RESET;
        waitForEnter();
        return;
    }

    const Resistor* resistor = nullptr;
    const Capacitor* capacitor = nullptr;
    const VoltageSource* source = nullptr;
    int resistorCount = 0;
    int capacitorCount = 0;
    int dcSourceCount = 0;

    for (const auto& componentPtr : circuit->getComponents()) {
        const Component* component = componentPtr.get();

        if (const auto* r = dynamic_cast<const Resistor*>(component)) {
            resistor = r;
            ++resistorCount;
        }
        else if (const auto* c = dynamic_cast<const Capacitor*>(component)) {
            capacitor = c;
            ++capacitorCount;
        }
        else if (const auto* v = dynamic_cast<const VoltageSource*>(component)) {
            if (!v->getIsAC()) {
                source = v;
                ++dcSourceCount;
            }
        }
    }

    const int nodeCount = circuit->getHighestNode() + 1;

    if (resistorCount != 1 || capacitorCount != 1 ||
        dcSourceCount != 1 || nodeCount != 3) {
        std::cout
            << RED
            << "Bang-bang control currently supports a simple first-order RC circuit.\n\n"
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

    if (resistor == nullptr || capacitor == nullptr || source == nullptr) {
        std::cout << RED << "The RC components could not be identified.\n" << RESET;
        waitForEnter();
        return;
    }

    const double R = resistor->getResistance();
    const double C = capacitor->getCapacitance();
    const double maximumOutputVoltage = source->getVoltage(0.0);
    const double minimumOutputVoltage = 0.0;
    const double timeConstant = R * C;

    if (R <= 0.0 || C <= 0.0 || maximumOutputVoltage <= minimumOutputVoltage) {
        std::cout << RED << "The RC circuit or source values are invalid.\n" << RESET;
        waitForEnter();
        return;
    }

    std::cout
        << GREEN << "Simple RC circuit detected.\n" << RESET << '\n'
        << "Resistance:             " << R << " ohms\n"
        << "Capacitance:            " << C << " F\n"
        << "Maximum output voltage: " << maximumOutputVoltage << " V\n"
        << "Minimum output voltage: " << minimumOutputVoltage << " V\n"
        << "Time constant:          " << timeConstant << " s\n\n";

    double targetVoltage = 0.0;
    double initialVoltage = 0.0;
    double stopTime = 0.0;
    double timeStep = 0.0;

    auto readDouble = [](const char* prompt, double& value) {
        std::cout << prompt;
        if (std::cin >> value) {
            return true;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    };

    if (!readDouble("Target voltage (V): ", targetVoltage) ||
        !readDouble("Initial capacitor voltage (V): ", initialVoltage) ||
        !readDouble("Simulation stop time (s): ", stopTime) ||
        !readDouble("Simulation time step (s): ", timeStep)) {
        std::cout << RED << "\nInvalid numeric input.\n" << RESET;
        waitForEnter();
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!std::isfinite(targetVoltage) ||
        !std::isfinite(initialVoltage) ||
        !std::isfinite(stopTime) ||
        !std::isfinite(timeStep)) {
        std::cout << RED << "\nSimulation values must be finite.\n" << RESET;
        waitForEnter();
        return;
    }

    if (targetVoltage < minimumOutputVoltage ||
        targetVoltage > maximumOutputVoltage) {
        std::cout
            << RED
            << "\nTarget voltage must be between "
            << minimumOutputVoltage << " V and "
            << maximumOutputVoltage << " V.\n"
            << RESET;
        waitForEnter();
        return;
    }

    if (stopTime <= 0.0 || timeStep <= 0.0 || timeStep > stopTime) {
        std::cout
            << RED
            << "\nStop time and time step must be positive, and the time step "
            << "must not exceed the stop time.\n"
            << RESET;
        waitForEnter();
        return;
    }

    try {
        Controller controller(
            targetVoltage,
            maximumOutputVoltage,
            minimumOutputVoltage
        );

        auto controlledRCEquation =
            [R, C, &controller](double /*time*/, double capacitorVoltage) {
                const double inputVoltage =
                    controller.calculateOutput(capacitorVoltage);
                return (inputVoltage - capacitorVoltage) / (R * C);
            };

        showHeader("Running Bang-Bang Controller Simulation");
        std::cout << "Solving controlled RC differential equation...\n" << std::flush;

        FirstOrderODESolver solver;
        solver.solveEuler(
            controlledRCEquation,
            0.0,
            initialVoltage,
            timeStep,
            stopTime
        );

        const auto& timeValues = solver.getXValues();
        const auto& voltageValues = solver.getYValues();

        if (timeValues.empty() || voltageValues.empty()) {
            throw std::runtime_error("The ODE solver returned no simulation data.");
        }
        if (timeValues.size() != voltageValues.size()) {
            throw std::runtime_error("The transient X and Y vectors have different sizes.");
        }

        Series capacitorSeries;
        capacitorSeries.name = "Capacitor Voltage";
        capacitorSeries.y = voltageValues;
        capacitorSeries.symbol = 'C';
        capacitorSeries.color = PlotColor::Cyan;

        Series targetSeries;
        targetSeries.name = "Target Voltage";
        targetSeries.y.assign(voltageValues.size(), controller.getTargetVoltage());
        targetSeries.symbol = 'T';
        targetSeries.color = PlotColor::Green;

        std::vector<Series> series = {capacitorSeries, targetSeries};
        Plot plot(SCREEN_WIDTH, "s", "V");

        showHeader("Bang-Bang Controller Results");
        std::cout
            << GREEN
            << "Bang-bang controller simulation completed successfully.\n"
            << RESET << '\n'
            << "R:                 " << R << " ohms\n"
            << "C:                 " << C << " F\n"
            << "Target voltage:    " << controller.getTargetVoltage() << " V\n"
            << "Maximum output:    " << controller.getMaximumOutputVoltage() << " V\n"
            << "Minimum output:    " << controller.getMinimumOutputVoltage() << " V\n"
            << "Time constant:     " << timeConstant << " s\n"
            << "Initial voltage:   " << initialVoltage << " V\n"
            << "Simulation length: " << stopTime << " s\n"
            << "Time step:         " << timeStep << " s\n"
            << "Calculated points: " << timeValues.size() << "\n\n";

        std::cout << plot.plot(timeValues, series);
    }
    catch (const std::exception& error) {
        showHeader("Bang-Bang Controller Error");
        std::cout
            << RED
            << "Bang-bang controller simulation failed.\n\n"
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
        std::cout << RED << "No circuit is currently loaded.\n" << RESET;
        waitForEnter();
        return;
    }

    const Resistor* resistor = nullptr;
    const Capacitor* capacitor = nullptr;
    const Inductor* inductor = nullptr;
    const VoltageSource* source = nullptr;

    int resistorCount = 0;
    int capacitorCount = 0;
    int inductorCount = 0;
    int dcSourceCount = 0;

    for (const auto& componentPtr : circuit->getComponents()) {
        const Component* component = componentPtr.get();

        if (const auto* r = dynamic_cast<const Resistor*>(component)) {
            resistor = r;
            ++resistorCount;
        }
        else if (const auto* c = dynamic_cast<const Capacitor*>(component)) {
            capacitor = c;
            ++capacitorCount;
        }
        else if (const auto* l = dynamic_cast<const Inductor*>(component)) {
            inductor = l;
            ++inductorCount;
        }
        else if (const auto* v = dynamic_cast<const VoltageSource*>(component)) {
            if (!v->getIsAC()) {
                source = v;
                ++dcSourceCount;
            }
        }
    }

    const bool isRC =
        resistorCount == 1 && capacitorCount == 1 &&
        inductorCount == 0 && dcSourceCount == 1;

    const bool isRL =
        resistorCount == 1 && capacitorCount == 0 &&
        inductorCount == 1 && dcSourceCount == 1;

    const bool isRLC =
        resistorCount == 1 && capacitorCount == 1 &&
        inductorCount == 1 && dcSourceCount == 1;

    if (!isRC && !isRL && !isRLC) {
        std::cout
            << RED
            << "Transient analysis supports simple series RC, RL or RLC circuits.\n\n"
            << RESET
            << "Detected components:\n"
            << "  Resistors:          " << resistorCount << '\n'
            << "  Capacitors:         " << capacitorCount << '\n'
            << "  Inductors:          " << inductorCount << '\n'
            << "  DC voltage sources: " << dcSourceCount << '\n';

        waitForEnter();
        return;
    }

    const double R = resistor->getResistance();
    const double C = capacitor == nullptr ? 0.0 : capacitor->getCapacitance();
    const double L = inductor == nullptr ? 0.0 : inductor->getInductance();
    const double Vs = source->getVoltage(0.0);

    if (R <= 0.0 ||
        (capacitor != nullptr && C <= 0.0) ||
        (inductor != nullptr && L <= 0.0)) {
        std::cout << RED << "Circuit component values must be positive.\n" << RESET;
        waitForEnter();
        return;
    }

    constexpr double PI = 3.14159265358979323846;
    double recommendedTimeStep = 0.0;

    std::cout
        << GREEN
        << "Supported transient circuit detected.\n"
        << RESET
        << '\n';

    if (isRC) {
        const double timeConstant = R * C;
        recommendedTimeStep = timeConstant / 20.0;

        std::cout
            << "Circuit type:       Series RC\n"
            << "Source voltage:     " << Vs << " V\n"
            << "Resistance:         " << R << " ohms\n"
            << "Capacitance:        " << C << " F\n"
            << "Time constant:      " << timeConstant << " s\n"
            << "Suggested timestep: less than "
            << recommendedTimeStep << " s\n\n";
    }
    else if (isRL) {
        const double timeConstant = L / R;
        recommendedTimeStep = timeConstant / 20.0;

        std::cout
            << "Circuit type:       Series RL\n"
            << "Source voltage:     " << Vs << " V\n"
            << "Resistance:         " << R << " ohms\n"
            << "Inductance:         " << L << " H\n"
            << "Time constant:      " << timeConstant << " s\n"
            << "Final DC current:   " << Vs / R << " A\n"
            << "Suggested timestep: less than "
            << recommendedTimeStep << " s\n\n";
    }
    else {
        const double naturalAngularFrequency =
            1.0 / std::sqrt(L * C);
        const double naturalFrequency =
            naturalAngularFrequency / (2.0 * PI);
        const double naturalPeriod =
            1.0 / naturalFrequency;
        const double dampingFactor =
            R / (2.0 * L);

        recommendedTimeStep = naturalPeriod / 100.0;

        std::cout
            << "Circuit type:       Series RLC\n"
            << "Source voltage:     " << Vs << " V\n"
            << "Resistance:         " << R << " ohms\n"
            << "Inductance:         " << L << " H\n"
            << "Capacitance:        " << C << " F\n"
            << "Natural frequency:  " << naturalFrequency << " Hz\n"
            << "Natural period:     " << naturalPeriod << " s\n"
            << "Damping factor:     " << dampingFactor << " 1/s\n"
            << "Suggested timestep: less than "
            << recommendedTimeStep << " s\n\n";
    }

    double initialVoltage = 0.0;
    double initialCurrent = 0.0;
    double stopTime = 0.0;
    double timeStep = 0.0;

    auto readDouble = [](const char* prompt, double& value) {
        std::cout << prompt;

        if (std::cin >> value) {
            return true;
        }

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
        return false;
    };

    if ((isRC || isRLC) &&
        !readDouble("Initial capacitor voltage (V): ", initialVoltage)) {
        std::cout << RED << "\nInvalid initial capacitor voltage.\n" << RESET;
        waitForEnter();
        return;
    }

    if ((isRL || isRLC) &&
        !readDouble("Initial inductor current (A): ", initialCurrent)) {
        std::cout << RED << "\nInvalid initial inductor current.\n" << RESET;
        waitForEnter();
        return;
    }

    if (!readDouble("Simulation stop time (s): ", stopTime) ||
        !readDouble("Simulation time step (s): ", timeStep)) {
        std::cout << RED << "\nInvalid simulation setting.\n" << RESET;
        waitForEnter();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (!std::isfinite(initialVoltage) ||
        !std::isfinite(initialCurrent) ||
        !std::isfinite(stopTime) ||
        !std::isfinite(timeStep) ||
        stopTime <= 0.0 ||
        timeStep <= 0.0 ||
        timeStep > stopTime) {
        std::cout << RED << "\nInvalid transient analysis settings.\n" << RESET;
        waitForEnter();
        return;
    }

    if (timeStep > recommendedTimeStep) {
        std::cout
            << YELLOW
            << "\nWarning: the selected timestep may be too large.\n"
            << "A timestep below " << recommendedTimeStep
            << " s is recommended for this circuit.\n"
            << RESET;
        waitForEnter();
    }

    try {
        if (isRC) {
            auto rcEquation =
                [R, C, Vs](double /*time*/, double capacitorVoltage) {
                    return (Vs - capacitorVoltage) / (R * C);
                };

            FirstOrderODESolver solver;
            solver.solveEuler(
                rcEquation,
                0.0,
                initialVoltage,
                timeStep,
                stopTime
            );

            const auto& timeValues = solver.getXValues();
            const auto& capacitorVoltageValues = solver.getYValues();

            std::vector<double> resistorVoltageValues;
            resistorVoltageValues.reserve(capacitorVoltageValues.size());

            for (const double capacitorVoltage : capacitorVoltageValues) {
                resistorVoltageValues.push_back(Vs - capacitorVoltage);
            }

            Series resistorSeries;
            resistorSeries.name = "Resistor Voltage";
            resistorSeries.y = resistorVoltageValues;
            resistorSeries.symbol = 'R';
            resistorSeries.color = PlotColor::Magenta;

            Series capacitorSeries;
            capacitorSeries.name = "Capacitor Voltage";
            capacitorSeries.y = capacitorVoltageValues;
            capacitorSeries.symbol = 'C';
            capacitorSeries.color = PlotColor::Cyan;

            Plot voltagePlot(SCREEN_WIDTH, "s", "V");

            showHeader("RC Transient Results");
            std::cout
                << "Voltage across each passive component:\n\n"
                << voltagePlot.plot(
                    timeValues,
                    std::vector<Series>{resistorSeries, capacitorSeries}
                );
        }
        else if (isRL) {
            auto rlEquation =
                [R, L, Vs](double /*time*/, double current) {
                    return (Vs - R * current) / L;
                };

            FirstOrderODESolver solver;
            solver.solveEuler(
                rlEquation,
                0.0,
                initialCurrent,
                timeStep,
                stopTime
            );

            const auto& timeValues = solver.getXValues();
            const auto& currentValues = solver.getYValues();

            std::vector<double> resistorVoltageValues;
            std::vector<double> inductorVoltageValues;
            resistorVoltageValues.reserve(currentValues.size());
            inductorVoltageValues.reserve(currentValues.size());

            for (const double current : currentValues) {
                const double resistorVoltage = R * current;
                const double inductorVoltage = Vs - resistorVoltage;

                resistorVoltageValues.push_back(resistorVoltage);
                inductorVoltageValues.push_back(inductorVoltage);
            }

            Series resistorSeries;
            resistorSeries.name = "Resistor Voltage";
            resistorSeries.y = resistorVoltageValues;
            resistorSeries.symbol = 'R';
            resistorSeries.color = PlotColor::Magenta;

            Series inductorSeries;
            inductorSeries.name = "Inductor Voltage";
            inductorSeries.y = inductorVoltageValues;
            inductorSeries.symbol = 'L';
            inductorSeries.color = PlotColor::Yellow;

            Series currentSeries;
            currentSeries.name = "Inductor Current";
            currentSeries.y = currentValues;
            currentSeries.symbol = 'I';
            currentSeries.color = PlotColor::Cyan;

            Plot plot(SCREEN_WIDTH, "s", "V / A");

            showHeader("RL Transient Results");
            std::cout
                << "RL circuit response:\n\n"
                << plot.plot(
                    timeValues,
                    std::vector<Series>{
                        resistorSeries,
                        inductorSeries,
                        currentSeries
                    }
                );
        }
        else {
            auto rlcEquations =
                [R, L, C, Vs](
                    double /*time*/,
                    double capacitorVoltage,
                    double current,
                    double& voltageDerivative,
                    double& currentDerivative
                ) {
                    voltageDerivative = current / C;
                    currentDerivative =
                        (Vs - R * current - capacitorVoltage) / L;
                };

            SecondOrderODESolver solver;
            solver.solveEuler(
                rlcEquations,
                0.0,
                initialVoltage,
                initialCurrent,
                timeStep,
                stopTime
            );

            const auto& timeValues = solver.getTimeValues();
            const auto& capacitorVoltageValues = solver.getFirstStateValues();
            const auto& currentValues = solver.getSecondStateValues();

            if (timeValues.size() != capacitorVoltageValues.size() ||
                timeValues.size() != currentValues.size()) {
                throw std::runtime_error(
                    "The RLC solver returned mismatched result vectors."
                );
            }

            std::vector<double> resistorVoltageValues;
            std::vector<double> inductorVoltageValues;
            resistorVoltageValues.reserve(currentValues.size());
            inductorVoltageValues.reserve(currentValues.size());

            for (std::size_t i = 0; i < currentValues.size(); ++i) {
                const double resistorVoltage = R * currentValues[i];
                const double inductorVoltage =
                    Vs - resistorVoltage - capacitorVoltageValues[i];

                resistorVoltageValues.push_back(resistorVoltage);
                inductorVoltageValues.push_back(inductorVoltage);
            }

            Series resistorSeries;
            resistorSeries.name = "Resistor Voltage";
            resistorSeries.y = resistorVoltageValues;
            resistorSeries.symbol = 'R';
            resistorSeries.color = PlotColor::Magenta;

            Series inductorSeries;
            inductorSeries.name = "Inductor Voltage";
            inductorSeries.y = inductorVoltageValues;
            inductorSeries.symbol = 'L';
            inductorSeries.color = PlotColor::Yellow;

            Series capacitorSeries;
            capacitorSeries.name = "Capacitor Voltage";
            capacitorSeries.y = capacitorVoltageValues;
            capacitorSeries.symbol = 'C';
            capacitorSeries.color = PlotColor::Cyan;

            Series currentSeries;
            currentSeries.name = "Inductor Current";
            currentSeries.y = currentValues;
            currentSeries.symbol = 'I';
            currentSeries.color = PlotColor::Green;

            std::vector<Series> series = {
                resistorSeries,
                inductorSeries,
                capacitorSeries,
                currentSeries
            };

            Plot plot(
                SCREEN_WIDTH,
                "s",
                "V / A"
            );

            showHeader("RLC Transient Results");

            std::cout
                << "RLC circuit response:\n\n"
                << plot.plot(
                    timeValues,
                    series
                );
        }
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
