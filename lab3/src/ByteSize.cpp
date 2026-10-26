#include "ByteSize.hpp"

#include <cstdio>
#include "Exceptions.hpp"

namespace {
constexpr uint64_t kBytesPerKiB = 1024ULL;
constexpr uint64_t kBytesPerMiB = 1024ULL * 1024ULL;
constexpr uint64_t kBytesPerGiB = 1024ULL * 1024ULL * 1024ULL;
constexpr std::size_t kBufferSize = 64;
}

ByteSize::ByteSize(uint64_t bytes) : bytes_(bytes) {}

ByteSize ByteSize::fromKiB(uint64_t kib) {
    return ByteSize(kib * kBytesPerKiB);
}

ByteSize ByteSize::fromMiB(uint64_t mib) {
    return ByteSize(mib * kBytesPerMiB);
}

ByteSize ByteSize::fromGiB(uint64_t gib) {
    return ByteSize(gib * kBytesPerGiB);
}

double ByteSize::toKiB() const {
    return static_cast<double>(bytes_) / static_cast<double>(kBytesPerKiB);
}

double ByteSize::toMiB() const {
    return static_cast<double>(bytes_) / static_cast<double>(kBytesPerMiB);
}

double ByteSize::toGiB() const {
    return static_cast<double>(bytes_) / static_cast<double>(kBytesPerGiB);
}

ByteSize ByteSize::plus(const ByteSize& other) const {
    return ByteSize(bytes_ + other.bytes_);
}

ByteSize ByteSize::minus(const ByteSize& other) const {
    if (bytes_ < other.bytes_) {
        throw InvalidConfigurationException("ByteSize subtraction underflow");
    }
    return ByteSize(bytes_ - other.bytes_);
}

ByteSize ByteSize::times(uint32_t factor) const {
    return ByteSize(bytes_ * factor);
}

bool ByteSize::isGreaterThan(const ByteSize& other) const {
    return bytes_ > other.bytes_;
}

std::string ByteSize::toString() const {
    char buffer[kBufferSize];
    if (bytes_ >= kBytesPerGiB) {
        std::snprintf(buffer, sizeof(buffer), "%.2f GiB", toGiB());
    } else if (bytes_ >= kBytesPerMiB) {
        std::snprintf(buffer, sizeof(buffer), "%.2f MiB", toMiB());
    } else if (bytes_ >= kBytesPerKiB) {
        std::snprintf(buffer, sizeof(buffer), "%.2f KiB", toKiB());
    } else {
        std::snprintf(buffer, sizeof(buffer), "%llu B", static_cast<unsigned long long>(bytes_));
    }
    return std::string(buffer);
}
