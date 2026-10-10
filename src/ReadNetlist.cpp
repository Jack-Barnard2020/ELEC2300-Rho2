/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: ReadNetlist.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for reading and parsing circuit netlists.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */


#include "ReadNetlist.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"
#include "Resistor.hpp"
#include "VoltageSource.hpp"

#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

ReadNetlist::ReadNetlist(const std::string& filename) : filename(filename) {}

Circuit ReadNetlist::read() const {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open netlist: " + filename);
    }
    Circuit circuit;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(inputFile, line)) {
        ++lineNumber;
        line = removeComment(line);
        if (!tokenise(line).empty()) {
            parseLine(line, lineNumber, circuit);
        }
    }
    if (circuit.getNumberOfComponents() == 0) {
        throw std::runtime_error("Netlist contains no components.");
    }
    return circuit;
}

void ReadNetlist::parseLine(const std::string& line, std::size_t lineNumber, Circuit& circuit) const {
    const auto tokens = tokenise(line);
    auto fail = [lineNumber](const std::string& message) {
        throw std::runtime_error("Netlist line " + std::to_string(lineNumber) + ": " + message);
    };
    if (tokens.size() < 5) fail("expected at least 5 fields");

    try {
        const std::string& name = tokens[0];
        std::size_t used1 = 0, used2 = 0;
        const int node1 = std::stoi(tokens[1], &used1);
        const int node2 = std::stoi(tokens[2], &used2);
        if (used1 != tokens[1].size() || used2 != tokens[2].size()) fail("invalid node number");
        const std::string& type = tokens[3];

        if (type == "R" || type == "C" || type == "L" || type == "VDC") {
            if (tokens.size() != 5) fail(type + " requires exactly 5 fields");
            const double value = parseValue(tokens[4]);
            if (type == "R") circuit.addComponent(std::make_unique<Resistor>(name, node1, node2, value));
            else if (type == "C") circuit.addComponent(std::make_unique<Capacitor>(name, node1, node2, value));
            else if (type == "L") circuit.addComponent(std::make_unique<Inductor>(name, node1, node2, value));
            else circuit.addComponent(std::make_unique<VoltageSource>(name, node1, node2, value));
        } else if (type == "VAC") {
            if (tokens.size() != 7) fail("VAC requires 7 fields: name node1 node2 VAC amplitude frequency phase");
            circuit.addComponent(std::make_unique<VoltageSource>(
                name, node1, node2, parseValue(tokens[4]), parseValue(tokens[5]), parseValue(tokens[6])));
        } else {
            fail("unknown component type '" + type + "'");
        }
    } catch (const std::runtime_error&) {
        throw;
    } catch (const std::exception& error) {
        fail(error.what());
    }
}

std::vector<std::string> ReadNetlist::tokenise(const std::string& line) const {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token) tokens.push_back(token);
    return tokens;
}

std::string ReadNetlist::removeComment(const std::string& line) const {
    const auto position = line.find('#');
    return position == std::string::npos ? line : line.substr(0, position);
}

double ReadNetlist::parseValue(const std::string& token) const {
    std::size_t consumed = 0;
    const double number = std::stod(token, &consumed);
    if (consumed == token.size()) return number;

    static const std::unordered_map<std::string, double> scale = {
        {"T", 1e12}, {"G", 1e9}, {"M", 1e6}, {"k", 1e3},
        {"m", 1e-3}, {"u", 1e-6}, {"n", 1e-9}, {"p", 1e-12}
    };
    const std::string suffix = token.substr(consumed);
    const auto found = scale.find(suffix);
    if (found == scale.end()) {
        throw std::invalid_argument("invalid numeric value '" + token + "'");
    }
    return number * found->second;
}
