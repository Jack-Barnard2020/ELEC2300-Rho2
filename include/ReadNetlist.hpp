#pragma once

#include "Circuit.hpp"

#include <cstddef>
#include <string>
#include <vector>

// Reads a text netlist and constructs the corresponding circuit.
class ReadNetlist {
public:
    explicit ReadNetlist(const std::string& filename);

    [[nodiscard]] Circuit read() const;

private:
    void parseLine(const std::string& line, std::size_t lineNumber, Circuit& circuit) const;
    [[nodiscard]] std::vector<std::string> tokenise(const std::string& line) const;
    [[nodiscard]] std::string removeComment(const std::string& line) const;
    [[nodiscard]] double parseValue(const std::string& token) const;

    std::string filename;
};
