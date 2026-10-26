#include "MacAddress.hpp"

#include <cstdio>
#include <sstream>
#include "Exceptions.hpp"

namespace {
constexpr std::size_t kMacOctetCount = 6;
constexpr std::size_t kBufferSize = 32;
}

MacAddress::MacAddress(std::vector<uint8_t> octets) : octets_(std::move(octets)) {
    if (octets_.size() != kMacOctetCount) {
        throw InvalidAddressException("MAC address must consist of exactly 6 octets");
    }
}

MacAddress MacAddress::fromString(const std::string& str) {
    std::vector<uint8_t> parsed;
    std::stringstream ss(str);
    std::string segment;
    while (std::getline(ss, segment, ':')) {
        if (segment.empty() || segment.length() > 2) {
            throw InvalidAddressException("Invalid MAC segment: " + segment);
        }
        try {
            const unsigned long val = std::stoul(segment, nullptr, 16);
            if (val > 0xFF) {
                throw InvalidAddressException("MAC octet out of range");
            }
            parsed.push_back(static_cast<uint8_t>(val));
        } catch (...) {
            throw InvalidAddressException("Failed to parse MAC octet: " + segment);
        }
    }
    if (parsed.size() != kMacOctetCount) {
        throw InvalidAddressException("Invalid MAC format: " + str);
    }
    return MacAddress(parsed);
}

std::string MacAddress::toString() const {
    char buffer[kBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
                  octets_[0], octets_[1], octets_[2], octets_[3], octets_[4], octets_[5]);
    return std::string(buffer);
}

bool MacAddress::isBroadcast() const {
    for (uint8_t b : octets_) {
        if (b != 0xFF) {
            return false;
        }
    }
    return true;
}

bool MacAddress::isMulticast() const {
    return (octets_[0] & 0x01) == 0x01;
}

bool MacAddress::matches(const MacAddress& other) const {
    return octets_ == other.octets_;
}
