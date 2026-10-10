/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: ReadNetlist.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for reading netlist functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#include "ReadNetlist.hpp"

#include "Capacitor.hpp"
#include "Inductor.hpp"
#include "Resistor.hpp"
#include "VoltageSource.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

ReadNetlist::ReadNetlist(
    const std::string& filename
)
    : filename(filename) {
}

Circuit ReadNetlist::read() const {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        throw std::runtime_error(
            "Could not open netlist: " + filename
        );
    }

    Circuit circuit;
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(inputFile, line)) {
        ++lineNumber;

        line = removeComment(line);

        if (tokenise(line).empty()) {
            continue;
        }

        parseLine(line, lineNumber, circuit);
    }

    return circuit;
}

void ReadNetlist::parseLine(
    const std::string& line,
    std::size_t lineNumber,
    Circuit& circuit
) const {
    const std::vector<std::string> tokens =
        tokenise(line);

    // Expected:
    // name node1 node2 type value
    //
    // AC source:
    // name node1 node2 VAC amplitude frequency phase

    // TODO: Validate the token count.

    // TODO: Extract the name.

    // TODO: Convert the two node tokens into integers.

    // TODO: Extract the component type.

    // TODO: Parse the component value.

    // TODO:
    // Use std::make_unique to construct the correct
    // component and add it to the circuit.

    // Example:
    //
    // circuit.addComponent(
    //     std::make_unique<Resistor>(
    //         name, node1, node2, value
    //     )
    // );

    (void)lineNumber;
    (void)circuit;
}

std::vector<std::string> ReadNetlist::tokenise(
    const std::string& line
) const {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string ReadNetlist::removeComment(
    const std::string& line
) const {
    const std::size_t commentPosition =
        line.find('#');

    if (commentPosition == std::string::npos) {
        return line;
    }

    return line.substr(0, commentPosition);
}

double ReadNetlist::parseValue(
    const std::string& token
) const {
    // TODO: Add engineering suffix support.
    // For now, std::stod handles values such as:
    // 1000
    // 1e-6
    // 2.5

    return std::stod(token);
}

/*
[ ] Check the number of tokens on each line.
[ ] Parse component names.
[ ] Parse node numbers.
[ ] Parse component types.
[ ] Construct resistor objects for R.
[ ] Construct capacitor objects for C.
[ ] Construct inductor objects for L.
[ ] Construct DC sources for VDC.
[ ] Construct AC sources for VAC.
[ ] Report errors with line numbers.
[ ] Support engineering suffixes such as k, M, m, u, n and p.
[ ] Decide whether component type
*/