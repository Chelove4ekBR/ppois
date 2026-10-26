#include "UninterruptiblePowerSupply.hpp"

#include "Exceptions.hpp"

namespace {
constexpr uint32_t kMaxBattery = 100;
constexpr double kBaseDischargeRatePerMinute = 2.0;
}

UninterruptiblePowerSupply::UninterruptiblePowerSupply(uint32_t capacityVa, uint32_t connectedLoadWatt,
                                                       uint32_t batteryPercent)
    : capacityVa_(capacityVa), batteryPercent_(batteryPercent),
      onBattery_(false), connectedLoadWatt_(connectedLoadWatt) {
    if (capacityVa_ == 0 || batteryPercent_ > kMaxBattery) {
        throw InvalidConfigurationException("Invalid UPS parameters");
    }
}

void UninterruptiblePowerSupply::simulatePowerOutage() {
    onBattery_ = true;
}

void UninterruptiblePowerSupply::restoreMainsPower() {
    onBattery_ = false;
}

double UninterruptiblePowerSupply::estimatedRuntimeMinutes() const {
    if (connectedLoadWatt_ == 0) {
        return 999.0;
    }
    const double batteryFraction = static_cast<double>(batteryPercent_) / 100.0;
    const double loadFactor = static_cast<double>(capacityVa_) / static_cast<double>(connectedLoadWatt_);
    return batteryFraction * loadFactor * 10.0;
}

void UninterruptiblePowerSupply::chargeBattery(uint32_t percent) {
    batteryPercent_ += percent;
    if (batteryPercent_ > kMaxBattery) {
        batteryPercent_ = kMaxBattery;
    }
}

void UninterruptiblePowerSupply::discharge(uint32_t minutes) {
    if (!onBattery_) {
        return;
    }
    const uint32_t drop = static_cast<uint32_t>(static_cast<double>(minutes) * kBaseDischargeRatePerMinute);
    if (drop >= batteryPercent_) {
        batteryPercent_ = 0;
        throw BatteryDepletedException("UPS battery completely depleted: computer emergency shutdown");
    }
    batteryPercent_ -= drop;
}
