/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Plotter.hpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Header file for the plotting functionality.
    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include <string>
#include <vector>

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

struct Series {
    std::vector<double> y;
    std::string name;
    char symbol;
    PlotColor color = PlotColor::Default;
};


class Plot {
    private:
        int width;

        std::string xUnits;
        std::string yUnits;
    public:
        Plot(
            int width,
            const std::string& xUnits,
            const std::string& yUnits
        );

        std::string plot(
            const std::vector<double>& x,
            const std::vector<Series>& series
        ) const;
};