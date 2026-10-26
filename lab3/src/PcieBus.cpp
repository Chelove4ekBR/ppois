#include "PcieBus.hpp"

#include "Exceptions.hpp"

PcieBus::PcieBus(uint32_t maxTotalLanes) : maxTotalLanes_(maxTotalLanes) {
    if (maxTotalLanes_ == 0) {
        throw InvalidConfigurationException("PCIe bus max lanes must be positive");
    }
}

void PcieBus::addSlot(const PcieSlot& slot) {
    slots_.push_back(slot);
}

uint32_t PcieBus::allocateSlot(const std::string& device, uint32_t lanes) {
    if (totalOccupiedLanes() + lanes > maxTotalLanes_) {
        throw IncompatibleHardwareException("Not enough total PCIe bus lanes available");
    }
    for (auto& slot : slots_) {
        if (slot.canAccommodateLanes(lanes)) {
            slot.plugDevice(device, lanes);
            return slot.slotId();
        }
    }
    throw PortOccupiedException("No available PCIe slot found for device " + device);
}

void PcieBus::releaseSlot(uint32_t slotId) {
    auto& slot = findSlot(slotId);
    slot.unplugDevice();
}

uint32_t PcieBus::totalOccupiedLanes() const {
    uint32_t total = 0;
    for (const auto& slot : slots_) {
        if (slot.isOccupied()) {
            total += slot.laneCount();
        }
    }
    return total;
}

bool PcieBus::hasAvailableSlot(uint32_t lanes) const {
    if (totalOccupiedLanes() + lanes > maxTotalLanes_) {
        return false;
    }
    for (const auto& slot : slots_) {
        if (slot.canAccommodateLanes(lanes)) {
            return true;
        }
    }
    return false;
}

PcieSlot& PcieBus::findSlot(uint32_t slotId) {
    for (auto& slot : slots_) {
        if (slot.slotId() == slotId) {
            return slot;
        }
    }
    throw ComponentNotFoundException("PCIe slot not found: " + std::to_string(slotId));
}

const PcieSlot& PcieBus::findSlot(uint32_t slotId) const {
    for (const auto& slot : slots_) {
        if (slot.slotId() == slotId) {
            return slot;
        }
    }
    throw ComponentNotFoundException("PCIe slot not found: " + std::to_string(slotId));
}
