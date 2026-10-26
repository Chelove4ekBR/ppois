#include "RamModule.hpp"

#include "Exceptions.hpp"

namespace {
constexpr double kDualChannelMultiplier = 2.0;
}

RamModule::RamModule(uint32_t maxSlots)
    : maxSlots_(maxSlots), operatingFrequency_(ClockFrequency(0)) {
    if (maxSlots_ == 0) {
        throw InvalidConfigurationException("RAM module must have at least one slot");
    }
}

void RamModule::installStick(const RamStick& stick) {
    if (isFull()) {
        throw PortOccupiedException("All RAM slots are occupied (" + std::to_string(maxSlots_) + ")");
    }
    if (!sticks_.empty() && !stick.matchesGeneration(sticks_.front().generation())) {
        throw IncompatibleHardwareException("Cannot mix RAM generations: existing " +
                                            sticks_.front().generation() + ", new " + stick.generation());
    }
    sticks_.push_back(stick);
    recalculateFrequency();
}

void RamModule::removeStick(const std::string& serialNumber) {
    for (auto it = sticks_.begin(); it != sticks_.end(); ++it) {
        if (it->serialNumber() == serialNumber) {
            sticks_.erase(it);
            recalculateFrequency();
            return;
        }
    }
    throw ComponentNotFoundException("RAM stick with serial " + serialNumber + " not found");
}

ByteSize RamModule::totalCapacity() const {
    ByteSize total(0);
    for (const auto& stick : sticks_) {
        total = total.plus(stick.capacity());
    }
    return total;
}

uint32_t RamModule::installedCount() const {
    return static_cast<uint32_t>(sticks_.size());
}

bool RamModule::isFull() const {
    return sticks_.size() >= maxSlots_;
}

bool RamModule::isMultiChannel() const {
    return sticks_.size() >= 2;
}

double RamModule::totalBandwidthMBs() const {
    if (sticks_.empty()) {
        return 0.0;
    }
    double baseBandwidth = sticks_.front().calculateBandwidthMBs();
    if (isMultiChannel()) {
        baseBandwidth *= kDualChannelMultiplier;
    }
    return baseBandwidth;
}

void RamModule::recalculateFrequency() {
    if (sticks_.empty()) {
        operatingFrequency_ = ClockFrequency(0);
        return;
    }
    ClockFrequency minFreq = sticks_.front().frequency();
    for (const auto& stick : sticks_) {
        if (minFreq.isGreaterThan(stick.frequency())) {
            minFreq = stick.frequency();
        }
    }
    operatingFrequency_ = minFreq;
}
