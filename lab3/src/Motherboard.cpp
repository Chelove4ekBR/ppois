#include "Motherboard.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kDefaultSlot1Gen = 4;
constexpr uint32_t kDefaultSlot1Lanes = 16;
constexpr uint32_t kDefaultSlot2Gen = 4;
constexpr uint32_t kDefaultSlot2Lanes = 4;
}

Motherboard::Motherboard(std::string model, std::string formFactor, std::string socket,
                         Chipset chipset, Bios bios, uint32_t maxRamSlots)
    : model_(std::move(model)), formFactor_(std::move(formFactor)), socket_(std::move(socket)),
      chipset_(std::move(chipset)), bios_(std::move(bios)), pcieBus_(48),
      maxRamSlots_(maxRamSlots), cpu_(nullptr), powerConnected_(false) {
    if (maxRamSlots_ == 0) {
        throw InvalidConfigurationException("Motherboard must have at least one RAM slot");
    }
    pcieBus_.addSlot(PcieSlot(1, kDefaultSlot1Gen, kDefaultSlot1Lanes));
    pcieBus_.addSlot(PcieSlot(2, kDefaultSlot2Gen, kDefaultSlot2Lanes));
}

void Motherboard::installCpu(CpuPtr cpu) {
    if (!cpu) {
        throw ComponentNotFoundException("Cannot install null CPU pointer");
    }
    if (cpu->socket() != socket_) {
        throw IncompatibleHardwareException("CPU socket " + cpu->socket() +
                                            " does not match motherboard socket " + socket_);
    }
    if (!chipset_.supportsSocket(cpu->socket())) {
        throw IncompatibleHardwareException("Chipset does not support CPU socket " + cpu->socket());
    }
    cpu_ = std::move(cpu);
}

CpuPtr Motherboard::removeCpu() {
    if (!cpu_) {
        throw ComponentNotFoundException("No CPU installed on motherboard");
    }
    CpuPtr extracted = std::move(cpu_);
    cpu_ = nullptr;
    return extracted;
}

void Motherboard::connectPower(const PowerSupplyUnit& psu) {
    if (psu.ratedWattage() == 0) {
        throw InvalidConfigurationException("PSU has zero wattage");
    }
    powerConnected_ = true;
}

void Motherboard::disconnectPower() {
    powerConnected_ = false;
}

bool Motherboard::isReadyToBoot(bool hasRam) const {
    if (!powerConnected_) {
        return false;
    }
    const bool hasCpu = (cpu_ != nullptr);
    return bios_.performPost(hasCpu, hasRam);
}

uint32_t Motherboard::installPcieDevice(const std::string& deviceName, uint32_t lanes) {
    return pcieBus_.allocateSlot(deviceName, lanes);
}
