#include "Cpu.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kHeatingPerLoad = 0.25;
constexpr double kMaxLoad = 100.0;
}

Cpu::Cpu(std::string model, std::string socket, uint32_t coreCount, ClockFrequency baseClock,
         ByteSize l3Cache, uint32_t tdpWatt)
    : model_(std::move(model)), socket_(std::move(socket)),
      l3CacheSize_(l3Cache), tdpWatt_(tdpWatt), thermalZone_(35.0, 85.0, 100.0),
      cooler_(nullptr), paste_() {
    if (coreCount == 0) {
        throw InvalidConfigurationException("Cpu must have at least one core");
    }
    cores_.reserve(coreCount);
    for (uint32_t i = 0; i < coreCount; ++i) {
        cores_.emplace_back(i, baseClock);
    }
}

uint32_t Cpu::totalCores() const {
    return static_cast<uint32_t>(cores_.size());
}

uint32_t Cpu::activeCoresCount() const {
    uint32_t count = 0;
    for (const auto& core : cores_) {
        if (core.isActive()) {
            ++count;
        }
    }
    return count;
}

double Cpu::averageUsage() const {
    const uint32_t activeCount = activeCoresCount();
    if (activeCount == 0) {
        return 0.0;
    }
    double sum = 0.0;
    for (const auto& core : cores_) {
        if (core.isActive()) {
            sum += core.usagePercent();
        }
    }
    return sum / static_cast<double>(activeCount);
}

void Cpu::attachCooler(CpuCoolerPtr cooler) {
    if (!cooler) {
        throw ComponentNotFoundException("Null cooler pointer");
    }
    cooler_ = std::move(cooler);
}

void Cpu::setCoreUsage(uint32_t coreIndex, double percent) {
    if (coreIndex >= cores_.size()) {
        throw ComponentNotFoundException("Core index out of range: " + std::to_string(coreIndex));
    }
    cores_[coreIndex].setUsage(percent);
}

void Cpu::setCoreActive(uint32_t coreIndex, bool active) {
    if (coreIndex >= cores_.size()) {
        throw ComponentNotFoundException("Core index out of range: " + std::to_string(coreIndex));
    }
    cores_[coreIndex].setActive(active);
}

double Cpu::executeTask(uint64_t totalCycles) {
    const uint32_t activeCount = activeCoresCount();
    if (activeCount == 0) {
        throw ComponentNotFoundException("No active CPU cores available");
    }
    const uint64_t cyclesPerCore = totalCycles / activeCount;
    double maxTime = 0.0;
    for (auto& core : cores_) {
        if (core.isActive()) {
            const double coreTime = core.executeCycles(cyclesPerCore);
            if (coreTime > maxTime) {
                maxTime = coreTime;
            }
        }
    }
    const double heatIncrease = (averageUsage() / kMaxLoad) * static_cast<double>(tdpWatt_) * kHeatingPerLoad;
    thermalZone_.updateTemperature(heatIncrease);
    coolDown();
    return maxTime;
}

void Cpu::coolDown() {
    if (cooler_) {
        const double delta = cooler_->calculateCoolingDelta(thermalZone_.temperatureC(), tdpWatt_);
        const double pasteFactor = paste_.effectiveConductivity() / paste_.thermalConductivity();
        thermalZone_.cool(delta * pasteFactor);
    }
}

bool Cpu::isOverheating() const {
    return thermalZone_.isThrottling() || thermalZone_.isCritical();
}

void Cpu::overclockAllCores(double percent) {
    for (auto& core : cores_) {
        const ClockFrequency boosted = core.baseFrequency().overclock(percent);
        core = CpuCore(core.coreId(), boosted);
    }
}
