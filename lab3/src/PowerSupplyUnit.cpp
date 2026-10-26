#include "PowerSupplyUnit.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kPercentBase = 100.0;
constexpr double kBaseEfficiency = 80.0;
constexpr double kOptimalEfficiency = 92.0;
constexpr double kOptimalLoadMin = 40.0;
constexpr double kOptimalLoadMax = 70.0;
}

PowerSupplyUnit::PowerSupplyUnit(std::string model, uint32_t ratedWattage,
                                 std::string efficiencyRating, bool isModular)
    : model_(std::move(model)), ratedWattage_(ratedWattage),
      efficiencyRating_(std::move(efficiencyRating)), currentDrawWatt_(0), isModular_(isModular) {
    if (ratedWattage_ == 0) {
        throw InvalidConfigurationException("PSU rated wattage must be greater than zero");
    }
}

void PowerSupplyUnit::drawPower(uint32_t watts) {
    const uint64_t nextDraw = static_cast<uint64_t>(currentDrawWatt_) + static_cast<uint64_t>(watts);
    if (nextDraw > static_cast<uint64_t>(ratedWattage_)) {
        throw PowerSupplyOverloadException("Power supply wattage exceeded: requested " +
                                           std::to_string(nextDraw) + " W, rated " +
                                           std::to_string(ratedWattage_) + " W");
    }
    currentDrawWatt_ += watts;
}

void PowerSupplyUnit::releasePower(uint32_t watts) {
    if (watts > currentDrawWatt_) {
        currentDrawWatt_ = 0;
    } else {
        currentDrawWatt_ -= watts;
    }
}

bool PowerSupplyUnit::isOverloaded() const {
    return currentDrawWatt_ >= ratedWattage_;
}

uint32_t PowerSupplyUnit::remainingCapacityWatt() const {
    if (currentDrawWatt_ >= ratedWattage_) {
        return 0;
    }
    return ratedWattage_ - currentDrawWatt_;
}

double PowerSupplyUnit::loadPercentage() const {
    return (static_cast<double>(currentDrawWatt_) * kPercentBase) / static_cast<double>(ratedWattage_);
}

double PowerSupplyUnit::calculateEfficiency() const {
    const double load = loadPercentage();
    if (load >= kOptimalLoadMin && load <= kOptimalLoadMax) {
        return kOptimalEfficiency;
    }
    return kBaseEfficiency;
}
