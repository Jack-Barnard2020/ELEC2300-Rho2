#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller {
public:
    Controller(
        double targetVoltage,
        double maximumOutputVoltage,
        double minimumOutputVoltage
    );

    double calculateOutput(double currentVoltage) const;

    double getTargetVoltage() const;
    double getMaximumOutputVoltage() const;
    double getMinimumOutputVoltage() const;

private:
    double targetVoltage;
    double maximumOutputVoltage;
    double minimumOutputVoltage;
};

#endif
