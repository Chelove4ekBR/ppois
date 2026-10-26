#include "NetworkConnection.hpp"

#include <utility>
#include "Exceptions.hpp"

NetworkConnection::NetworkConnection(uint32_t connectionId, IpAddress remoteIp,
                                     uint16_t remotePort, std::string state)
    : connectionId_(connectionId), remoteIp_(std::move(remoteIp)),
      remotePort_(remotePort), state_(std::move(state)), bytesTransferred_(0) {
    if (remotePort_ == 0) {
        throw InvalidConfigurationException("Port number cannot be zero");
    }
}

void NetworkConnection::transmitData(uint32_t bytes) {
    if (!isOpen()) {
        throw NetworkException("Cannot transmit data over closed connection #" + std::to_string(connectionId_));
    }
    bytesTransferred_ += bytes;
}

void NetworkConnection::close() {
    state_ = "CLOSED";
}

bool NetworkConnection::isOpen() const {
    return state_ == "ESTABLISHED";
}

void NetworkConnection::reopen(uint16_t newPort) {
    if (newPort == 0) {
        throw InvalidConfigurationException("Port cannot be zero");
    }
    remotePort_ = newPort;
    state_ = "ESTABLISHED";
}

std::string NetworkConnection::describe() const {
    return "Connection #" + std::to_string(connectionId_) + " to " +
           remoteIp_.toString() + ":" + std::to_string(remotePort_) + " [" + state_ + "]";
}
