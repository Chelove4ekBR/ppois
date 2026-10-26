#include "ThermalZone.hpp"

#include "Exceptions.hpp"

namespace {
constexpr double kMinAllowedTemp = -50.0;
constexpr double kAbsoluteZero = -273.15;
}

ThermalZone::ThermalZone(double initialTempC, double throttleTempC, double criticalTempC)
    : temperatureC_(initialTempC), throttleTempC_(throttleTempC), criticalTempC_(criticalTempC) {
    if (throttleTempC_ >= criticalTempC_ || throttleTempC_ < kMinAllowedTemp) {
        throw InvalidConfigurationException("Invalid thermal zone threshold configuration");
    }
    if (temperatureC_ >= criticalTempC_) {
        throw OverheatingException("Initial temperature exceeds critical limit");
    }
}

void ThermalZone::updateTemperature(double deltaC) {
    const double nextTemp = temperatureC_ + deltaC;
    setTemperature(nextTemp);
}

void ThermalZone::setTemperature(double tempC) {
    if (tempC < kAbsoluteZero) {
        throw InvalidConfigurationException("Temperature cannot be below absolute zero");
    }
    temperatureC_ = tempC;
    if (isCritical()) {
        throw OverheatingException("ThermalZone reached critical temperature: " + std::to_string(temperatureC_) + " C");
    }
}

void ThermalZone::cool(double deltaC) {
    if (deltaC < 0.0) {
        throw InvalidConfigurationException("Cooling delta must be non-negative");
    }
    const double nextTemp = temperatureC_ - deltaC;
    temperatureC_ = nextTemp < kMinAllowedTemp ? kMinAllowedTemp : nextTemp;
}

bool ThermalZone::isThrottling() const {
    return temperatureC_ >= throttleTempC_;
}

bool ThermalZone::isCritical() const {
    return temperatureC_ >= criticalTempC_;
}

double ThermalZone::headroomC() const {
    return criticalTempC_ - temperatureC_;
}
