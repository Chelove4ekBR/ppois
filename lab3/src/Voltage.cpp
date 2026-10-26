#include "Voltage.hpp"

#include <cstdio>
#include "Exceptions.hpp"

namespace {
constexpr double kMilliToVolts = 1000.0;
constexpr std::size_t kBufferSize = 32;
}

Voltage::Voltage(uint32_t millivolts) : millivolts_(millivolts) {}

double Voltage::toVolts() const {
    return static_cast<double>(millivolts_) / kMilliToVolts;
}

bool Voltage::isSafe(const Voltage& minSafe, const Voltage& maxSafe) const {
    return millivolts_ >= minSafe.millivolts_ && millivolts_ <= maxSafe.millivolts_;
}

Voltage Voltage::stepUp(uint32_t deltaMv) const {
    return Voltage(millivolts_ + deltaMv);
}

Voltage Voltage::stepDown(uint32_t deltaMv) const {
    if (millivolts_ < deltaMv) {
        throw InvalidConfigurationException("Voltage cannot be negative");
    }
    return Voltage(millivolts_ - deltaMv);
}

bool Voltage::isGreaterThan(const Voltage& other) const {
    return millivolts_ > other.millivolts_;
}

std::string Voltage::toString() const {
    char buffer[kBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%.3f V", toVolts());
    return std::string(buffer);
}
