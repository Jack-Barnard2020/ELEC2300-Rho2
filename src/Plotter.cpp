/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Plotter.cpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Implementation file for terminal plotting functionality.

    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
        2026/10/10 - Improved readability and documentation
   ====================================== */

#include "Plotter.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// Helper functions used only by this source file.
namespace {

// Convert a PlotColor into its ANSI terminal colour code.
const char* colorCode(PlotColor color)
{
    switch (color) {
        case PlotColor::Red:     return "\033[31m";
        case PlotColor::Green:   return "\033[32m";
        case PlotColor::Yellow:  return "\033[33m";
        case PlotColor::Blue:    return "\033[34m";
        case PlotColor::Magenta: return "\033[35m";
        case PlotColor::Cyan:    return "\033[36m";
        case PlotColor::White:   return "\033[37m";
        case PlotColor::Default: return "";
    }

    return "";
}

// Return a series symbol with its terminal colour applied.
// The reset code prevents the colour affecting later output.
std::string coloredSymbol(const Series& waveform)
{
    const char* code = colorCode(waveform.color);

    if (*code == '\0') {
        return std::string(1, waveform.symbol);
    }

    return std::string(code) + waveform.symbol + "\033[0m";
}

} // namespace

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
    // A plot requires at least one X value and one data series.
    if (x.empty() || series.empty()) {
        std::cerr << "Error: no data to plot.\n";
        return "";
    }

    // Width represents the number of columns available for plotting.
    if (width <= 0) {
        std::cerr << "Error: plot width must be greater than zero.\n";
        return "";
    }

    // Every series uses the same X axis, so each series must contain
    // exactly one Y value for every X value.
    for (const Series& waveform : series) {
        if (waveform.y.size() != x.size()) {
            std::cerr
                << "Error: X and Y vector lengths do not match.\n";
            return "";
        }
    }

    // Find the total Y range across every series. This gives all
    // waveforms a common scale, allowing them to be compared directly.
    double minY = series[0].y[0];
    double maxY = series[0].y[0];

    for (const Series& waveform : series) {
        for (double value : waveform.y) {
            if (value < minY) {
                minY = value;
            }

            if (value > maxY) {
                maxY = value;
            }
        }
    }

    std::ostringstream output;

    // ----- Legend -----
    // Keep the legend order identical to the order used when displaying
    // the numerical Y values beside each plotted row.
    for (const Series& waveform : series) {
        output
            << coloredSymbol(waveform)
            << " : "
            << waveform.name
            << '\n';
    }

    output << '\n';

    // ----- Plot data -----
    // One terminal row is produced for every X value.
    for (std::size_t i = 0; i < x.size(); ++i) {
        // Start with an empty row. A separate colour vector records
        // the colour required at each character position.
        std::string row(width, ' ');

        std::vector<PlotColor> rowColors(
            width,
            PlotColor::Default
        );

        // Add the Y value from each series to the current row.
        for (const Series& waveform : series) {
            const double y = waveform.y[i];
            int position = 0;

            if (maxY == minY) {
                // If the data has no Y range, every point would normally
                // map to the same edge. Centre it instead.
                position = width / 2;
            } else {
                // Convert Y into the range 0.0 to 1.0.
                const double normalised =
                    (y - minY) / (maxY - minY);

                // Scale the normalised value to a character position.
                position = static_cast<int>(
                    normalised * (width - 1)
                );
            }

            // Protect against positions outside the valid row range.
            if (position < 0) {
                position = 0;
            } else if (position >= width) {
                position = width - 1;
            }

            row[position] = waveform.symbol;
            rowColors[position] = waveform.color;
        }

        // Apply ANSI colours only after all data points have been
        // positioned. This keeps escape sequences out of the width maths.
        std::string coloredRow;

        for (std::size_t position = 0;
             position < row.size();
             ++position) {
            if (rowColors[position] == PlotColor::Default) {
                coloredRow += row[position];
            } else {
                coloredRow += colorCode(rowColors[position]);
                coloredRow += row[position];
                coloredRow += "\033[0m";
            }
        }

        // ----- X value and graphical row -----
        // X values use a fixed format so consecutive rows remain aligned.
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

        // Print the numerical Y values in the same order as the legend.
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

    // Display the scale used by the plot.
    output
        << "\nY range: "
        << minY
        << " "
        << yUnits
        << " to "
        << maxY
        << " "
        << yUnits
        << '\n';

    return output.str();
}
