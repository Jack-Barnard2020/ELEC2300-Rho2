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
#include <sstream>

namespace {

const char* colorCode(PlotColor color)
{
    switch (color) {
    case PlotColor::Red: return "\033[31m";
    case PlotColor::Green: return "\033[32m";
    case PlotColor::Yellow: return "\033[33m";
    case PlotColor::Blue: return "\033[34m";
    case PlotColor::Magenta: return "\033[35m";
    case PlotColor::Cyan: return "\033[36m";
    case PlotColor::White: return "\033[37m";
    case PlotColor::Default: return "";
    }

    return "";
}

std::string coloredSymbol(const Series& waveform)
{
    const char* code = colorCode(waveform.color);
    if (*code == '\0') {
        return std::string(1, waveform.symbol);
    }

    return std::string(code) + waveform.symbol + "\033[0m";
}

}

Plot::Plot(
    int width,
    const std::string& xUnits,
    const std::string& yUnits
)
    : width(width),
      xUnits(xUnits),
      yUnits(yUnits)
{}

std::string Plot::plot(
    const std::vector<double>& x,
    const std::vector<Series>& series
) const
{
    // Make sure there is data to plot
    if (x.empty() || series.empty()) {
        std::cerr << "Error: no data to plot.\n";
        return "";
    }

    // Make sure the plot has a valid width
    if (width <= 0) {
        std::cerr << "Error: plot width must be greater than zero.\n";
        return "";
    }

    // Make sure every waveform has one Y value
    // for every X value
    for (const Series& waveform : series) {
        if (waveform.y.size() != x.size()) {
            std::cerr
                << "Error: X and Y vector lengths do not match.\n";
            return "";
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

    std::ostringstream output;

    // Plot legend
    for (const Series& waveform : series)
    {
        output
            << coloredSymbol(waveform)
            << " : "
            << waveform.name
            << '\n';
    }

    output << '\n';


  
    // Plot every timestep
    for (std::size_t i = 0; i < x.size(); i++) {
        // Create one blank terminal row
        std::string row(width, ' ');
        std::vector<PlotColor> rowColors(
            width,
            PlotColor::Default
        );


        // Add every waveform to this row
        for (std::size_t seriesIndex = 0;
             seriesIndex < series.size();
             ++seriesIndex) {
            const Series& waveform = series[seriesIndex];
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


            if (position < 0) {
                position = 0;
            } else if (position >= width) {
                position = width - 1;
            }

            // Put the waveform symbol onto the row
            row[position] = waveform.symbol;
            rowColors[position] = waveform.color;
        }

        std::string coloredRow;
        for (std::size_t position = 0; position < row.size(); ++position) {
            if (rowColors[position] == PlotColor::Default) {
                coloredRow += row[position];
            } else {
                coloredRow += colorCode(rowColors[position]);
                coloredRow += row[position];
                coloredRow += "\033[0m";
            }
        }


        // Print this timestep, fixed to 2 decimal places, and 0 padded to 6 characters before the decimal point
        output
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
            << coloredRow
            << "|";

        // Print each Y value in the same order as the legend.
        for (const Series& waveform : series) {
            output
                << " "
                << coloredSymbol(waveform)
                << " = "
                << waveform.y[i]
                << " "
                << yUnits
                << "    ";
        }

        output << '\n';
    }


    // Print the Y range
    output << "\nY range: " << minY << " " << yUnits
           << " to " << maxY << " " << yUnits << '\n';

    return output.str();
}