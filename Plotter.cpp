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
#include <cmath>
#include <iomanip>
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
    // Make sure there is data to plot
    if (x.empty() || series.empty()) {
        std::cerr << "Error: no data to plot.\n";
        return;
    }

    // Make sure the plot has a valid width
    if (width <= 0) {
        std::cerr << "Error: plot width must be greater than zero.\n";
        return;
    }

    // Make sure every waveform has one Y value
    // for every X value
    for (const Series& waveform : series) {
        if (waveform.y.size() != x.size()) {
            std::cerr
                << "Error: X and Y vector lengths do not match.\n";
            return;
        }
    }

    double minY = series[0].y[0];
    double maxY = series[0].y[0];

    for (const Series& waveform : series) {
        for (double value : waveform.y) {
            if (value < minY) {minY = value;}
            if (value > maxY) {maxY = value;}
        }
    }

    // Plot Lengend 
    for (const Series& waveform : series)
    {
        std::cout
            << waveform.symbol
            << " : "
            << waveform.name
            << '\n';
    }

    std::cout << '\n';


  
    // Plot every timestep
    for (std::size_t i = 0; i < x.size(); i++) {
        // Create one blank terminal row
        std::string row(width, ' ');


        // Add every waveform to this row
        for (const Series& waveform : series) {
            double y = waveform.y[i];

            int position;


            // If every Y value is the same,
            // place the waveform in the middle
            if (maxY == minY) {position = width / 2;}
            else {
                // Convert Y into a value between 0 and 1
                double normalised =
                    (y - minY) / (maxY - minY);


                // Convert that into a terminal position
                position =
                    static_cast<int>(
                        normalised * (width - 1)
                    );
            }


            // Put the waveform symbol onto the row
            row[position] = waveform.symbol;
        }


        // Print this timestep, fixed to 2 decimal places, and 0 padded to 6 characters before the decimal point
        std::cout
            << std::fixed
            << std::setprecision(2)
            << (x[i] < 0.0 ? "-" : "+")
            << std::setw(9)
            << std::setfill('0')
            << std::abs(x[i])
            << std::setfill(' ')
            << " "
            << xUnits
            << " |"
            << row
            << "|";

        // Print each Y value in the same order as the legend.
        for (const Series& waveform : series) {
            std::cout
                << " "
                << waveform.symbol
                << " = "
                << waveform.y[i]
                << " "
                << yUnits
                << "    ";
        }

        std::cout << '\n';
    }


    // Print the Y range
    std::cout << "\nY range: " << minY << " " << yUnits << " to " << maxY << " " << yUnits << std::endl;
}