#include "NetworkPacket.hpp"

#include <utility>
#include "Exceptions.hpp"

NetworkPacket::NetworkPacket(IpAddress sourceIp, IpAddress destIp, uint32_t payloadBytes,
                             std::string protocol, uint8_t ttl)
    : sourceIp_(std::move(sourceIp)), destIp_(std::move(destIp)),
      payloadBytes_(payloadBytes), protocol_(std::move(protocol)),
      ttl_(ttl), checksum_(0) {
    if (protocol_.empty()) {
        throw InvalidConfigurationException("Network protocol cannot be empty");
    }
    checksum_ = calculateChecksum();
}

void NetworkPacket::decrementTtl() {
    if (ttl_ > 0) {
        --ttl_;
        checksum_ = calculateChecksum();
    }
}

bool NetworkPacket::isExpired() const {
    return ttl_ == 0;
}

uint16_t NetworkPacket::calculateChecksum() const {
    uint32_t sum = payloadBytes_ + ttl_;
    for (uint8_t b : sourceIp_.octets()) {
        sum += b;
    }
    for (uint8_t b : destIp_.octets()) {
        sum += b;
    }
    return static_cast<uint16_t>(sum & 0xFFFF);
}

bool NetworkPacket::verifyChecksum() const {
    return checksum_ == calculateChecksum();
}

std::string NetworkPacket::describe() const {
    return protocol_ + " packet " + sourceIp_.toString() + " -> " +
           destIp_.toString() + " (" + std::to_string(payloadBytes_) + " bytes, TTL=" +
           std::to_string(ttl_) + ")";
}
