#include "PcieSlot.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kGen3LaneBandwidthGbps = 0.985;
constexpr double kGen4LaneBandwidthGbps = 1.969;
constexpr double kGen5LaneBandwidthGbps = 3.938;
}

PcieSlot::PcieSlot(uint32_t slotId, uint32_t generation, uint32_t laneCount)
    : slotId_(slotId), generation_(generation), laneCount_(laneCount),
      occupied_(false), connectedDevice_("") {
    if (laneCount_ == 0 || generation_ < 1) {
        throw InvalidConfigurationException("Invalid PCIe slot specification");
    }
}

double PcieSlot::bandwidthGbps() const {
    double perLane = kGen3LaneBandwidthGbps;
    if (generation_ == 4) {
        perLane = kGen4LaneBandwidthGbps;
    } else if (generation_ >= 5) {
        perLane = kGen5LaneBandwidthGbps;
    }
    return perLane * static_cast<double>(laneCount_);
}

bool PcieSlot::isAvailable() const {
    return !occupied_;
}

bool PcieSlot::canAccommodateLanes(uint32_t lanes) const {
    return isAvailable() && (lanes <= laneCount_);
}

void PcieSlot::plugDevice(std::string device, uint32_t neededLanes) {
    if (occupied_) {
        throw PortOccupiedException("PCIe slot " + std::to_string(slotId_) + " is already occupied by " + connectedDevice_);
    }
    if (neededLanes > laneCount_) {
        throw IncompatibleHardwareException("Device requires " + std::to_string(neededLanes) +
                                            " lanes but slot has " + std::to_string(laneCount_));
    }
    connectedDevice_ = std::move(device);
    occupied_ = true;
}

void PcieSlot::unplugDevice() {
    if (!occupied_) {
        throw DeviceNotConnectedException("Slot " + std::to_string(slotId_) + " is empty");
    }
    occupied_ = false;
    connectedDevice_.clear();
}
