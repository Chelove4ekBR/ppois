#include "UsbPort.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kUsb2SpeedMbps = 480.0;
constexpr double kUsb3SpeedMbps = 5000.0;
constexpr double kUsbCSpeedMbps = 10000.0;
}

UsbPort::UsbPort(uint32_t portNumber, std::string version, uint32_t maxCurrentMa)
    : portNumber_(portNumber), version_(std::move(version)),
      maxCurrentMa_(maxCurrentMa), occupied_(false),
      connectedDevice_(""), consumedCurrentMa_(0) {
    if (maxCurrentMa_ == 0) {
        throw InvalidConfigurationException("USB max current must be positive");
    }
}

double UsbPort::speedMbps() const {
    if (version_ == "USB_2") {
        return kUsb2SpeedMbps;
    }
    if (version_ == "USB_C") {
        return kUsbCSpeedMbps;
    }
    return kUsb3SpeedMbps;
}

void UsbPort::plugDevice(std::string device, uint32_t currentMa) {
    if (occupied_) {
        throw PortOccupiedException("USB port #" + std::to_string(portNumber_) +
                                    " already occupied by " + connectedDevice_);
    }
    if (currentMa > maxCurrentMa_) {
        throw PowerSupplyOverloadException("Device " + device + " requires " + std::to_string(currentMa) +
                                           " mA, but port allows only " + std::to_string(maxCurrentMa_) + " mA");
    }
    connectedDevice_ = std::move(device);
    consumedCurrentMa_ = currentMa;
    occupied_ = true;
}

void UsbPort::unplugDevice() {
    if (!occupied_) {
        throw DeviceNotConnectedException("USB port #" + std::to_string(portNumber_) + " is empty");
    }
    occupied_ = false;
    connectedDevice_.clear();
    consumedCurrentMa_ = 0;
}

bool UsbPort::isAvailable() const {
    return !occupied_;
}

uint32_t UsbPort::availableCurrentMa() const {
    if (consumedCurrentMa_ >= maxCurrentMa_) {
        return 0;
    }
    return maxCurrentMa_ - consumedCurrentMa_;
}
