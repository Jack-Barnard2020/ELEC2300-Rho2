/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: fourierSeries.hpp
    Author: Jack Barnard
    Date: 2026/10/05
    Description: Header file for the Fourier series implementation. 
    Change Log:
        2026/10/09 - Initial commit (Jack Barnard)
   ====================================== */

#pragma once

#include <vector>

class fourierSeries {
    private:
        std::vector<double> sineCoefficents;
        std::vector<double> cosineCoefficents;

        double fundamentalFrequency;
        double timeStep;
        double runTime;

        std::vector<double> xValues;
        std::vector<double> yValues;
    public:
        fourierSeries(std::vector<double> sineCoefficents,
            std::vector<double> cosineCoefficents, 
            double fundamentalFrequency, 
            double timeStep, 
            double runTime);

        void generate();

        const std::vector<double>& getXValues() const;
        const std::vector<double>& getYValues() const;
};