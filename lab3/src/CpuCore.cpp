#include "CpuCore.hpp"

#include "Exceptions.hpp"

namespace {
constexpr double kMinUsage = 0.0;
constexpr double kMaxUsage = 100.0;
constexpr double kMinThrottle = 0.1;
constexpr double kMaxThrottle = 1.0;
}

CpuCore::CpuCore(uint32_t coreId, ClockFrequency baseFrequency)
    : coreId_(coreId), baseFrequency_(baseFrequency),
      currentFrequency_(baseFrequency), usagePercent_(0.0), active_(true) {}

void CpuCore::setActive(bool active) {
    active_ = active;
    if (!active_) {
        usagePercent_ = 0.0;
    }
}

void CpuCore::setUsage(double percent) {
    if (!active_) {
        throw InvalidConfigurationException("Cannot set usage on inactive core");
    }
    if (percent < kMinUsage || percent > kMaxUsage) {
        throw InvalidConfigurationException("Core usage must be between 0 and 100 percent");
    }
    usagePercent_ = percent;
}

void CpuCore::throttle(double factor) {
    if (factor < kMinThrottle || factor > kMaxThrottle) {
        throw InvalidConfigurationException("Throttle factor must be between 0.1 and 1.0");
    }
    const uint64_t throttledHertz = static_cast<uint64_t>(static_cast<double>(baseFrequency_.hertz()) * factor);
    currentFrequency_ = ClockFrequency(throttledHertz);
}

void CpuCore::resetFrequency() {
    currentFrequency_ = baseFrequency_;
}

double CpuCore::executeCycles(uint64_t cycles) {
    if (!active_) {
        throw ComponentNotFoundException("Core " + std::to_string(coreId_) + " is disabled");
    }
    if (currentFrequency_.hertz() == 0) {
        throw InvalidConfigurationException("Core frequency is zero");
    }
    return static_cast<double>(cycles) / static_cast<double>(currentFrequency_.hertz());
}
