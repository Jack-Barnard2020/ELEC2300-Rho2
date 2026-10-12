/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Circuit simulator with plotter and UI (potentially a GUI)
    Change Log:
        2026/10/05 - Initial commit (Jack Barnard)
   ====================================== */

#include "Circuit.hpp"
#include "NodalAnalysis.hpp"
#include "ReadNetlist.hpp"

#include <exception>
#include <iostream>
#include <vector>

int main() {
    try {
        ReadNetlist reader("example.net");

        Circuit circuit = reader.read();

        std::cout
            << "Components loaded: "
            << circuit.getNumberOfComponents()
            << '\n';

        NodalAnalysis analysis(circuit);

        const std::vector<double> voltages =
            analysis.solve();

        for (std::size_t i = 0;
             i < voltages.size();
             ++i) {

            std::cout
                << "Node " << i + 1
                << ": " << voltages[i]
                << " V\n";
        }
    }
    catch (const std::exception& error) {
        std::cerr
            << "Error: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}