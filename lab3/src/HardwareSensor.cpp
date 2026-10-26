#include "HardwareSensor.hpp"

#include <cmath>
#include <cstdio>
#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr std::size_t kBufferSize = 64;
constexpr double kPercentBase = 100.0;
}

HardwareSensor::HardwareSensor(std::string sensorName, std::string sensorType,
                               double currentValue, double thresholdValue)
    : sensorName_(std::move(sensorName)), sensorType_(std::move(sensorType)),
      currentValue_(currentValue), thresholdValue_(thresholdValue) {
    if (sensorName_.empty() || sensorType_.empty()) {
        throw InvalidConfigurationException("Sensor name and type cannot be empty");
    }
}

void HardwareSensor::updateValue(double val) {
    currentValue_ = val;
}

bool HardwareSensor::isAlarm() const {
    return currentValue_ >= thresholdValue_;
}

double HardwareSensor::deviationPercent() const {
    if (std::fabs(thresholdValue_) < 1e-6) {
        return 0.0;
    }
    return ((currentValue_ - thresholdValue_) / thresholdValue_) * kPercentBase;
}

std::string HardwareSensor::describe() const {
    char buffer[kBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%s (%s): %.2f / %.2f %s",
                  sensorName_.c_str(), sensorType_.c_str(), currentValue_, thresholdValue_,
                  isAlarm() ? "[ALARM]" : "[OK]");
    return std::string(buffer);
}
