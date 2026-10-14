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
   ====================================== */

// Include the header files written for this project.
#include "CLI.hpp"
#include "ReadNetlist.hpp"
#include "VoltageSource.hpp"
#include "NodalAnalysis.hpp"
#include "FirstOrderODESolver.hpp"
#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Plotter.hpp"

// Include standard library headers.
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>


// Keep terminal formatting constants local to this translation unit.
// An unnamed namespace was chosen instead of globals in CLI.hpp because these values are
// only an implementation detail of the CLI and do not need to be visible to other files.
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


// Main entry point for the command-line interface.
// Menu choices are read with getline rather than `cin >> choice`. This keeps menu input
// line-based and avoids leaving a newline behind for the next prompt.
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

// Clear the terminal and move the cursor back to the top-left.
// ANSI escape codes keep this simple and avoid platform-specific console APIs.
// The trade-off is that colour/clearing depends on the terminal supporting ANSI codes.
void CLI::clearScreen() const {
    std::cout << CLEAR_SCREEN << CURSOR_HOME << std::flush;
}

// Display a header with the program title, current netlist filename, and page title.
// The header is formatted with ANSI color codes for better readability.
// The page title is passed by const reference because it is read-only and does not need
// to be copied. Passing a std::string by value would also work, but gives no benefit here.
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

// Display a welcome message to the user when the program starts.
void CLI::showWelcome() const {
    showHeader("Welcome");

    std::cout
        << "This program loads an electrical circuit from a netlist\n"
        << "and provides command-line circuit analysis.\n";
}

// Display the main menu options to the user.
// Used as a separate function for better code organization and reusability.
void CLI::showMainMenu() const {
    showHeader("Main Menu");

    std::cout
        << "1. Load a netlist\n"
        << "2. Quit\n";
}

// Ask for a netlist path as a complete line, so paths containing spaces also work.
// A single Q/q is used as the cancel command rather than adding another menu level.
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

// Load the netlist file specified by the user and create a Circuit object.
// If the netlist is loaded successfully, display the component list and analysis menu.
// If an error occurs during loading, display an error message and return to the main menu.
void CLI::loadNetlist()
{
    const std::string filename = promptForNetlist();

    // Allow the user to cancel loading a netlist and return to the main menu.
    if (filename == "q" || filename == "Q") {
        return;
    }

    // Error handling for empty filename input.
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
        // Leave file parsing to ReadNetlist rather than doing it inside the CLI.
        // Keeping parsing and user-interface code separate makes both easier to change and test.
        ReadNetlist reader(filename);
        Circuit loadedCircuit = reader.read();

        // Move the finished Circuit into the optional instead of copying all of its components.
        // Copy assignment would be simpler-looking, but moving is appropriate because loadedCircuit
        // is only a temporary result and is not used afterwards.
        circuit = std::move(loadedCircuit);
        currentNetlistFilename = filename;

        showComponentList();
        analysisMenu();
    } catch (const std::exception& error) {
        showHeader("Error Loading Netlist");

        // Display the error message in red to indicate a failure in loading the netlist.
        std::cout
            << RED
            << "The netlist could not be loaded.\n\n"
            << RESET
            << error.what() 
            << '\n';

        waitForEnter();
    }
}

// Display the list of components in the currently loaded circuit.
void CLI::showComponentList() const {
    showHeader("Component List");

    // If no circuit is loaded, display an error message and return to the main menu.
    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // Display the component list in a formatted table.
    // The table includes the component number, name, type, nodes, value, frequency, and phase.
    // The frequency and phase are only displayed for voltage sources; other components show "-" in those columns.
    // Keep a const reference to the component collection. `auto` avoids repeating its long
    // smart-pointer container type, while the reference avoids copying the whole collection.
    const auto& components = circuit->getComponents();

    std::cout
        << GREEN
        << "Netlist loaded successfully.\n"
        << RESET
        << '\n';

    // Table headings with fixed widths for alignment.
    // The width may need to be adjusted based on the expected length of component names and types.
    // The width is repated again further down based on the empty space needed for each component and its values.

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

        // Components are stored through the common Component interface. A dynamic_cast is used
        // here because frequency and phase only exist on VoltageSource. A type flag or string
        // comparison could also be used, but the cast checks the real runtime type directly.
        const VoltageSource* voltageSource =
            dynamic_cast<const VoltageSource*>(&component);

        if (voltageSource != nullptr) {
            // Display frequency and phase for AC voltage sources; display "-" for DC voltage sources.
            // Uses the getters from the VoltageSource class to retrieve the frequency and phase values.
            if (voltageSource->getFrequency() != 0.0) {
                std::cout
                    << std::setw(16)
                    << (std::to_string(voltageSource->getFrequency()) + " Hz")
                    << voltageSource->getPhase()
                    << " deg";
            }
            else {
                // DC voltage source.
                // Display "-" for frequency and phase.
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

// Pause between screens so output does not disappear immediately when the next header clears
// the terminal. std::cin.get() is enough here because the earlier input code consumes pending newlines.
void CLI::waitForEnter() const {
    // Uses ANSI color codes to display the prompt in yellow for better visibility.
    std::cout
        << YELLOW
        << "\nPress Enter to continue..."
        << RESET;

    std::cin.get();
}

// Display the analysis menu and allow the user to select the type of analysis to perform.
// The user can choose between DC operating point analysis, transient analysis, returning to the main menu, or quitting the program.
// The menu will continue to be displayed until the user chooses to return to the main menu or quit.
void CLI::analysisMenu() {
    // This menu has its own flag as well as the class-wide `running` flag. That lets option 3
    // leave only this submenu, while option 4 can stop the entire CLI without using break/goto.
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

// Perform DC operating point analysis using nodal analysis.
// Uses the NodalAnalysis class to solve for node voltages in the circuit.
// Displays the results in a formatted table. If an error occurs during analysis, an error message is displayed.
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

        // `auto` keeps this independent of the exact container returned by solve(). It is stored
        // by value because this function owns the results it is about to display.
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

        // Ground (GND and commonly node 0) is always 0 V.
        std::cout
            << std::left
            << std::setw(12) << 0
            << 0.0
            << '\n';

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

// Perform transient analysis on a simple first-order RC circuit.
// Currently supports only circuits with one resistor, one capacitor, and one DC voltage source.
// The netlist is validated to ensure it meets these requirements before proceeding with the analysis.
// The user is prompted to enter the initial capacitor voltage, simulation stop time, and time step.
// The transient analysis is performed using a first-order ODE solver class.
// The results are displayed in the terminal using the graph plotter class.
// If an error occurs during the analysis, an error message is displayed.
void CLI::runTransientAnalysis() {
    showHeader("Transient Analysis");

    // Check if a circuit is loaded before proceeding with the analysis.
    if (!circuit.has_value()) {
        std::cout
            << RED
            << "No circuit is currently loaded.\n"
            << RESET;

        waitForEnter();
        return;
    }

    // Checks if the circuit contains exactly one resistor, one capacitor, and one DC voltage source.

    // Initialize pointers to the components and counters for each type of component.
    const Resistor* resistor = nullptr;
    const Capacitor* capacitor = nullptr;
    const VoltageSource* source = nullptr;

    // Counters for each type of component in the circuit.
    int resistorCount = 0;
    int capacitorCount = 0;
    int dcSourceCount = 0;

    for (const auto& componentPtr : circuit->getComponents()) {
        const Component* component = componentPtr.get();

        // dynamic_cast is used because the Circuit owns components through base-class pointers.
        // It is safer than a static_cast here: a failed cast simply returns nullptr.
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

    // This is deliberately defensive. The counts above should already prove that all three
    // pointers are valid, but keeping the check prevents accidental dereferencing if the
    // component-selection logic is changed later.
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

    // Cache RC because it is both useful to the user and repeatedly meaningful physically.
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

    // Numeric entry uses operator >> because it performs type checking for doubles. Unlike the
    // menu prompts, it leaves the final newline in the stream, so discard it before returning
    // to getline-based menus. Using getline + stod everywhere was another option, but would add
    // manual string-to-number parsing for no real advantage here.
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    // -----------------------------------------------------
    // Validate simulation parameters.
    // -----------------------------------------------------

    // Reject non-finite/invalid simulation settings before passing them to the solver.
    // These checks are kept in the CLI so bad user input is reported with a useful message.
    if (stopTime == std::numeric_limits<double>::infinity()) {
        std::cout
            << RED
            << "\nStop time cannot be infinity.\n"
            << RESET;

        waitForEnter();
        return;
    }

    if (stopTime == std::numeric_limits<double>::quiet_NaN()) {
        std::cout
            << RED
            << "\nStop time cannot be NaN.\n"
            << RESET;

        waitForEnter();
        return;
    }

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

    if (timeStep == std::numeric_limits<double>::infinity()) {
        std::cout
            << RED
            << "\nTime step cannot be infinity.\n"
            << RESET;

        waitForEnter();
        return;
    }

    if (timeStep == std::numeric_limits<double>::quiet_NaN()) {
        std::cout
            << RED
            << "\nTime step cannot be NaN.\n"
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

    // A lambda keeps this small equation next to the code that uses it. A separate named
    // function would also work, but then R, C and Vs would have to be passed or stored elsewhere.
    // Capture by value makes a stable copy of the circuit parameters for the solver callback.
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

        Series capacitorSeries, resistorSeries;

        capacitorSeries.name = "Capacitor Voltage";
        capacitorSeries.y = voltageValues;
        capacitorSeries.symbol = 'C';
        capacitorSeries.color = PlotColor::Cyan;

        resistorSeries.name = "Resistor Voltage";
        resistorSeries.symbol = 'R';
        resistorSeries.color = PlotColor::Magenta;
        // For this series RC case, KVL gives Vr = Vs - Vc. Reserve the final size first so the
        // vector does not repeatedly reallocate while points are appended.
        resistorSeries.y.reserve(voltageValues.size());
        for (const auto& vc : voltageValues) {
            resistorSeries.y.push_back(Vs - vc);
        }

        std::vector<Series> series = {
            capacitorSeries, resistorSeries
        };

        // -------------------------------------------------
        // Create the terminal plot.
        // -------------------------------------------------

        // A fixed width gives predictable terminal output. Automatically using the terminal
        // width would be more adaptive, but also needs platform-specific terminal-size handling.
        Plot plot(
            100,     // Plot width
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