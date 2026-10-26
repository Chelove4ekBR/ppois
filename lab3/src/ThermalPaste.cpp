#include "ThermalPaste.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kDegradationThresholdHours = 8760; // 1 год (365 * 24)
constexpr double kMaxDegradationFactor = 0.5;
}

ThermalPaste::ThermalPaste(std::string brand, double thermalConductivity)
    : brand_(std::move(brand)), thermalConductivity_(thermalConductivity), appliedHours_(0) {
    if (thermalConductivity_ <= 0.0) {
        throw InvalidConfigurationException("Thermal conductivity must be positive");
    }
}

void ThermalPaste::age(uint32_t hours) {
    appliedHours_ += hours;
}

bool ThermalPaste::isDegraded() const {
    return appliedHours_ >= kDegradationThresholdHours;
}

double ThermalPaste::effectiveConductivity() const {
    if (!isDegraded()) {
        return thermalConductivity_;
    }
    const double excessHours = static_cast<double>(appliedHours_ - kDegradationThresholdHours);
    const double drop = (excessHours / static_cast<double>(kDegradationThresholdHours)) * 0.2;
    const double factor = (drop > kMaxDegradationFactor) ? kMaxDegradationFactor : (1.0 - drop);
    return thermalConductivity_ * factor;
}

void ThermalPaste::replacePaste(std::string brand, double newConductivity) {
    if (newConductivity <= 0.0) {
        throw InvalidConfigurationException("New thermal conductivity must be positive");
    }
    brand_ = std::move(brand);
    thermalConductivity_ = newConductivity;
    appliedHours_ = 0;
}
