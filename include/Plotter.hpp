#pragma once

#include <string>
#include <vector>

// ANSI terminal colours available to a plotted series.
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

// One named set of Y values displayed using a symbol and optional colour.
struct Series {
    std::vector<double> y;
    std::string name;
    char symbol;
    PlotColor color = PlotColor::Default;
};

// Produces a horizontal terminal plot for a shared set of X values.
class Plot {
public:
    Plot(int width, const std::string& xUnits, const std::string& yUnits);

    [[nodiscard]] std::string plot(
        const std::vector<double>& x,
        const std::vector<Series>& series
    ) const;

private:
    int width;
    std::string xUnits;
    std::string yUnits;
};
