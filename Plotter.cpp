/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Plotter.cpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Implementation file for the plotting functionality.
    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
   ====================================== */

#include "Plotter.hpp"
#include <iostream>

Plot::Plot(
    int width,
    const std::string& xUnits,
    const std::string& yUnits
)
    : width(width),
      xUnits(xUnits),
      yUnits(yUnits)
{}


void Plot::plot(
    const std::vector<double>& x,
    const std::vector<Series>& series
) const
{
    // TODO:
    // Check all Series.y vectors
    // are the same length as x.


    // TODO:
    // Find minimum and maximum Y
    // across all Series.


    // TODO:
    // Print legend.


    // TODO:
    // For each x value:
    //
    // 1. Create a terminal row.
    //
    // 2. Convert each Series y value
    //    to a horizontal position.
    //
    // 3. Put its symbol in that position.
    //
    // 4. Print the row.
    //
    // X increases DOWN the terminal.
    // Y increases ACROSS the terminal.


    // TODO:
    // Print Y scale and units.
}