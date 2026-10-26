#include "ClockFrequency.hpp"

#include <cstdio>
#include "Exceptions.hpp"

namespace {
constexpr uint64_t kHertzPerKHz = 1000ULL;
constexpr uint64_t kHertzPerMHz = 1000ULL * 1000ULL;
constexpr uint64_t kHertzPerGHz = 1000ULL * 1000ULL * 1000ULL;
constexpr double kPercentBase = 100.0;
constexpr double kMinOverclockPercent = -100.0;
constexpr std::size_t kBufferSize = 64;
}

ClockFrequency::ClockFrequency(uint64_t hertz) : hertz_(hertz) {}

ClockFrequency ClockFrequency::fromMHz(uint64_t mhz) {
    return ClockFrequency(mhz * kHertzPerMHz);
}

ClockFrequency ClockFrequency::fromGHz(double ghz) {
    if (ghz < 0.0) {
        throw InvalidConfigurationException("Frequency cannot be negative");
    }
    return ClockFrequency(static_cast<uint64_t>(ghz * static_cast<double>(kHertzPerGHz)));
}

double ClockFrequency::toMHz() const {
    return static_cast<double>(hertz_) / static_cast<double>(kHertzPerMHz);
}

double ClockFrequency::toGHz() const {
    return static_cast<double>(hertz_) / static_cast<double>(kHertzPerGHz);
}

ClockFrequency ClockFrequency::overclock(double percent) const {
    if (percent < kMinOverclockPercent) {
        throw InvalidConfigurationException("Cannot reduce clock below 0 Hz");
    }
    const double factor = 1.0 + (percent / kPercentBase);
    const double newHertz = static_cast<double>(hertz_) * factor;
    return ClockFrequency(static_cast<uint64_t>(newHertz));
}

ClockFrequency ClockFrequency::plus(const ClockFrequency& other) const {
    return ClockFrequency(hertz_ + other.hertz_);
}

ClockFrequency ClockFrequency::minus(const ClockFrequency& other) const {
    if (hertz_ < other.hertz_) {
        throw InvalidConfigurationException("ClockFrequency underflow");
    }
    return ClockFrequency(hertz_ - other.hertz_);
}

bool ClockFrequency::isGreaterThan(const ClockFrequency& other) const {
    return hertz_ > other.hertz_;
}

std::string ClockFrequency::toString() const {
    char buffer[kBufferSize];
    if (hertz_ >= kHertzPerGHz) {
        std::snprintf(buffer, sizeof(buffer), "%.2f GHz", toGHz());
    } else if (hertz_ >= kHertzPerMHz) {
        std::snprintf(buffer, sizeof(buffer), "%.2f MHz", toMHz());
    } else if (hertz_ >= kHertzPerKHz) {
        std::snprintf(buffer, sizeof(buffer), "%.2f kHz", static_cast<double>(hertz_) / static_cast<double>(kHertzPerKHz));
    } else {
        std::snprintf(buffer, sizeof(buffer), "%llu Hz", static_cast<unsigned long long>(hertz_));
    }
    return std::string(buffer);
}
