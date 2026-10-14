/* =========== ELEC2300 - Rho2 ==========
    Project: Circuit Simulator
    File: Controller.cpp
    Author: Jack Barnard
    Date: 2026/10/14
    Description: Implementation file for bang-bang controller functionality.
    Change Log:
        2026/10/14 - Initial commit (Jack Barnard)
   ====================================== */

#include "Controller.hpp"
#include <stdexcept>

Controller::Controller(
    double targetVoltage,
    double maximumOutputVoltage,
    double minimumOutputVoltage
)
    : targetVoltage(targetVoltage),
      maximumOutputVoltage(maximumOutputVoltage),
      minimumOutputVoltage(minimumOutputVoltage)
{
    if (maximumOutputVoltage <= minimumOutputVoltage) {
        throw std::invalid_argument(
            "Maximum output voltage must be greater than minimum output voltage."
        );
    }

    if (targetVoltage < minimumOutputVoltage ||
        targetVoltage > maximumOutputVoltage) {
        throw std::invalid_argument(
            "Target voltage must be within the controller output range."
        );
    }
}

double Controller::calculateOutput(double currentVoltage) const
{
    if (currentVoltage < targetVoltage) {
        return maximumOutputVoltage;
    }

    return minimumOutputVoltage;
}

double Controller::getTargetVoltage() const
{
    return targetVoltage;
}

double Controller::getMaximumOutputVoltage() const
{
    return maximumOutputVoltage;
}

double Controller::getMinimumOutputVoltage() const
{
    return minimumOutputVoltage;
}
