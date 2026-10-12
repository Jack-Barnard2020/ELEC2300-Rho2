/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: ReadNetlist.hpp
    Author: Jack Barnard
    Date: 2026/10/10
    Description: Header file for reading netlist functionality.
    Change Log:
        2026/10/10 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include "Circuit.hpp"

#include <cstddef>
#include <string>
#include <vector>

class ReadNetlist {
private:
    std::string filename;

public:
    explicit ReadNetlist(
        const std::string& filename
    );

    Circuit read() const;

private:
    void parseLine(
        const std::string& line,
        std::size_t lineNumber,
        Circuit& circuit
    ) const;

    std::vector<std::string> tokenise(
        const std::string& line
    ) const;

    std::string removeComment(
        const std::string& line
    ) const;

    double parseValue(
        const std::string& token
    ) const;
};
