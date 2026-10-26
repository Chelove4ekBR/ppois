#include "RamStick.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kBytesPerTransfer = 8.0; // 64-бит шина = 8 байт
}

RamStick::RamStick(std::string serialNumber, ByteSize capacity, ClockFrequency frequency,
                   std::string generation, uint32_t casLatency, Voltage voltage)
    : serialNumber_(std::move(serialNumber)), capacity_(capacity),
      frequency_(frequency), generation_(std::move(generation)),
      casLatency_(casLatency), voltage_(voltage) {
    if (serialNumber_.empty() || capacity_.bytes() == 0) {
        throw InvalidConfigurationException("RAM stick must have valid serial number and non-zero capacity");
    }
}

double RamStick::calculateBandwidthMBs() const {
    const double mhz = frequency_.toMHz();
    return mhz * kBytesPerTransfer;
}

bool RamStick::matchesGeneration(const std::string& gen) const {
    return generation_ == gen;
}

bool RamStick::validateCompatibility(const Voltage& maxSafeVoltage) const {
    return !voltage_.isGreaterThan(maxSafeVoltage);
}

std::string RamStick::describe() const {
    return generation_ + " " + capacity_.toString() + " @" + frequency_.toString() +
           " CL" + std::to_string(casLatency_) + " (" + serialNumber_ + ")";
}
