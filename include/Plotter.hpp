/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Plotter.hpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Header file for terminal plotting functionality.

    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
        2026/10/10 - Improved readability and documentation
   ====================================== */

#pragma once

#include <string>
#include <vector>

// Colours available for plotting a series in the terminal.
// Default leaves the terminal's current colour unchanged.
enum class PlotColor {
    Default,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

// Represents one set of Y values to be displayed on a plot.
struct Series {
    std::vector<double> y;
    std::string name;
    char symbol;
    PlotColor color = PlotColor::Default;
};

// Produces a horizontal terminal plot from a shared set of X values
// and one or more data series.
class Plot {
private:
    // Number of character positions available inside the plot.
    int width;

    // Labels displayed alongside the X and Y values.
    std::string xUnits;
    std::string yUnits;

public:
    Plot(
        int width,
        const std::string& xUnits,
        const std::string& yUnits
    );

    // Creates the complete plot as a string.
    //
    // Each Series must contain the same number of Y values as there
    // are X values. All series share the same horizontal Y scale.
    std::string plot(
        const std::vector<double>& x,
        const std::vector<Series>& series
    ) const;
};