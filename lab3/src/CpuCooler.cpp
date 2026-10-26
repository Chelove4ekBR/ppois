#include "CpuCooler.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kPercentBase = 100.0;
constexpr double kCoolingBaseFactor = 0.4;
constexpr double kCoolingMinAmbient = 25.0;
}

CpuCooler::CpuCooler(std::string coolerType, uint32_t maxTdpWatt, uint32_t maxFanRpm)
    : coolerType_(std::move(coolerType)), maxTdpWatt_(maxTdpWatt), fanRpm_(1000),
      maxFanRpm_(maxFanRpm), pumpActive_(coolerType_ == "Liquid") {
    if (maxTdpWatt_ == 0 || maxFanRpm_ == 0) {
        throw InvalidConfigurationException("Cooler specs must be greater than zero");
    }
}

void CpuCooler::setFanSpeed(uint32_t rpm) {
    if (rpm > maxFanRpm_) {
        throw InvalidConfigurationException("Fan speed exceeds maximum RPM: " + std::to_string(maxFanRpm_));
    }
    fanRpm_ = rpm;
}

void CpuCooler::stopFan() {
    fanRpm_ = 0;
}

double CpuCooler::calculateCoolingDelta(double currentTemp, uint32_t tdpWatt) const {
    if (currentTemp <= kCoolingMinAmbient) {
        return 0.0;
    }
    const double speedFactor = static_cast<double>(fanRpm_) / static_cast<double>(maxFanRpm_);
    const double capacityFactor = (tdpWatt <= maxTdpWatt_) ? 1.0 : (static_cast<double>(maxTdpWatt_) / tdpWatt);
    return (currentTemp - kCoolingMinAmbient) * speedFactor * capacityFactor * kCoolingBaseFactor;
}

bool CpuCooler::isSufficientFor(uint32_t tdpWatt) const {
    return maxTdpWatt_ >= tdpWatt;
}

double CpuCooler::rpmPercentage() const {
    return (static_cast<double>(fanRpm_) * kPercentBase) / static_cast<double>(maxFanRpm_);
}
