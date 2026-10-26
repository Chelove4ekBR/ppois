#include "IpAddress.hpp"

#include <cstdio>
#include <sstream>
#include "Exceptions.hpp"

namespace {
constexpr std::size_t kIpv4OctetCount = 4;
constexpr uint8_t kMaxPrefix = 32;
constexpr std::size_t kBufferSize = 32;
}

IpAddress::IpAddress(std::vector<uint8_t> octets, uint8_t subnetPrefix)
    : octets_(std::move(octets)), subnetPrefix_(subnetPrefix) {
    if (octets_.size() != kIpv4OctetCount || subnetPrefix_ > kMaxPrefix) {
        throw InvalidAddressException("Invalid IPv4 address or subnet prefix");
    }
}

IpAddress IpAddress::fromString(const std::string& str) {
    std::string ipPart = str;
    uint8_t prefix = 24;
    const std::size_t slashPos = str.find('/');
    if (slashPos != std::string::npos) {
        ipPart = str.substr(0, slashPos);
        const std::string prefixStr = str.substr(slashPos + 1);
        try {
            prefix = static_cast<uint8_t>(std::stoul(prefixStr));
        } catch (...) {
            throw InvalidAddressException("Invalid subnet prefix in string: " + str);
        }
    }
    std::vector<uint8_t> octets;
    std::stringstream ss(ipPart);
    std::string seg;
    while (std::getline(ss, seg, '.')) {
        if (seg.empty()) {
            throw InvalidAddressException("Empty octet in IP address");
        }
        try {
            const unsigned long val = std::stoul(seg);
            if (val > 255) {
                throw InvalidAddressException("IP octet out of range: " + seg);
            }
            octets.push_back(static_cast<uint8_t>(val));
        } catch (...) {
            throw InvalidAddressException("Invalid IP octet: " + seg);
        }
    }
    if (octets.size() != kIpv4OctetCount) {
        throw InvalidAddressException("IPv4 address must contain 4 octets: " + str);
    }
    return IpAddress(octets, prefix);
}

std::string IpAddress::toString() const {
    char buffer[kBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u",
                  octets_[0], octets_[1], octets_[2], octets_[3]);
    return std::string(buffer);
}

std::string IpAddress::toStringWithPrefix() const {
    return toString() + "/" + std::to_string(subnetPrefix_);
}

bool IpAddress::isPrivate() const {
    // 10.0.0.0/8
    if (octets_[0] == 10) {
        return true;
    }
    // 172.16.0.0 - 172.31.255.255
    if (octets_[0] == 172 && octets_[1] >= 16 && octets_[1] <= 31) {
        return true;
    }
    // 192.168.0.0/16
    if (octets_[0] == 192 && octets_[1] == 168) {
        return true;
    }
    return false;
}

bool IpAddress::isLoopback() const {
    return octets_[0] == 127;
}

bool IpAddress::isInSameSubnet(const IpAddress& other) const {
    if (subnetPrefix_ != other.subnetPrefix_) {
        return false;
    }
    const IpAddress netA = networkAddress();
    const IpAddress netB = other.networkAddress();
    return netA.octets_ == netB.octets_;
}

IpAddress IpAddress::networkAddress() const {
    const uint32_t mask = (subnetPrefix_ == 0) ? 0 : (~0U << (32 - subnetPrefix_));
    uint32_t ipNum = (static_cast<uint32_t>(octets_[0]) << 24) |
                     (static_cast<uint32_t>(octets_[1]) << 16) |
                     (static_cast<uint32_t>(octets_[2]) << 8) |
                     static_cast<uint32_t>(octets_[3]);
    const uint32_t netNum = ipNum & mask;
    std::vector<uint8_t> netOctets = {
        static_cast<uint8_t>((netNum >> 24) & 0xFF),
        static_cast<uint8_t>((netNum >> 16) & 0xFF),
        static_cast<uint8_t>((netNum >> 8) & 0xFF),
        static_cast<uint8_t>(netNum & 0xFF)
    };
    return IpAddress(netOctets, subnetPrefix_);
}
