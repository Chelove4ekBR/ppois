#include "NetworkAdapter.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kMbpsPerGbps = 1000.0;
}

NetworkAdapter::NetworkAdapter(std::string adapterName, MacAddress mac,
                               IpAddress ip, uint32_t maxSpeedMbps)
    : adapterName_(std::move(adapterName)), macAddress_(std::move(mac)),
      ipAddress_(std::move(ip)), maxSpeedMbps_(maxSpeedMbps),
      linkUp_(true), totalBytesTransferred_(0) {
    if (adapterName_.empty() || maxSpeedMbps_ == 0) {
        throw InvalidConfigurationException("Invalid network adapter parameters");
    }
}

void NetworkAdapter::setIpAddress(IpAddress ip) {
    ipAddress_ = std::move(ip);
}

void NetworkAdapter::connectLink() {
    linkUp_ = true;
}

void NetworkAdapter::disconnectLink() {
    linkUp_ = false;
}

void NetworkAdapter::sendPacket(NetworkPacket& packet) {
    if (!linkUp_) {
        throw NetworkException("Cannot send packet: network interface " + adapterName_ + " link is down");
    }
    if (packet.isExpired()) {
        throw NetworkException("Cannot send expired network packet (TTL=0)");
    }
    if (!packet.verifyChecksum()) {
        throw NetworkException("Corrupted packet checksum detected on transmission");
    }
    packet.decrementTtl();
    totalBytesTransferred_ += packet.payloadBytes();
}

void NetworkAdapter::receivePacket(const NetworkPacket& packet) {
    if (!linkUp_) {
        throw NetworkException("Cannot receive packet: interface link is down");
    }
    if (!packet.verifyChecksum()) {
        throw NetworkException("Corrupted received packet checksum");
    }
    totalBytesTransferred_ += packet.payloadBytes();
}

double NetworkAdapter::bandwidthGbps() const {
    return static_cast<double>(maxSpeedMbps_) / kMbpsPerGbps;
}
