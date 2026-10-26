#include "UsbHub.hpp"

#include <utility>
#include "Exceptions.hpp"

UsbHub::UsbHub(std::string hubName, uint32_t portCount)
    : hubName_(std::move(hubName)) {
    if (hubName_.empty()) {
        throw InvalidConfigurationException("Hub name cannot be empty");
    }
    for (uint32_t i = 1; i <= portCount; ++i) {
        ports_.emplace_back(i, (i <= 2) ? "USB_C" : "USB_3", (i <= 2) ? 3000 : 900);
    }
}

void UsbHub::addPort(const UsbPort& port) {
    ports_.push_back(port);
}

void UsbHub::plugDevice(uint32_t portNumber, const std::string& device, uint32_t currentMa) {
    auto& port = findPort(portNumber);
    port.plugDevice(device, currentMa);
}

void UsbHub::unplugDevice(uint32_t portNumber) {
    auto& port = findPort(portNumber);
    port.unplugDevice();
}

uint32_t UsbHub::connectedDevicesCount() const {
    uint32_t count = 0;
    for (const auto& port : ports_) {
        if (port.isOccupied()) {
            ++count;
        }
    }
    return count;
}

uint32_t UsbHub::totalCurrentDrawMa() const {
    uint32_t sum = 0;
    for (const auto& port : ports_) {
        sum += port.consumedCurrentMa();
    }
    return sum;
}

UsbPort& UsbHub::findPort(uint32_t portNumber) {
    for (auto& port : ports_) {
        if (port.portNumber() == portNumber) {
            return port;
        }
    }
    throw ComponentNotFoundException("USB port #" + std::to_string(portNumber) + " not found in hub");
}

const UsbPort& UsbHub::findPort(uint32_t portNumber) const {
    for (const auto& port : ports_) {
        if (port.portNumber() == portNumber) {
            return port;
        }
    }
    throw ComponentNotFoundException("USB port #" + std::to_string(portNumber) + " not found in hub");
}
