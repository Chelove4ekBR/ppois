#include "DeviceDriver.hpp"

#include <utility>
#include "Exceptions.hpp"

DeviceDriver::DeviceDriver(std::string driverName, std::string deviceCategory, std::string version)
    : driverName_(std::move(driverName)), deviceCategory_(std::move(deviceCategory)),
      version_(std::move(version)), loaded_(false) {
    if (driverName_.empty() || version_.empty()) {
        throw InvalidConfigurationException("Driver name and version cannot be empty");
    }
}

void DeviceDriver::load() {
    if (loaded_) {
        return;
    }
    loaded_ = true;
}

void DeviceDriver::unload() {
    loaded_ = false;
}

bool DeviceDriver::sendIoctl(uint32_t commandCode) {
    if (!loaded_) {
        throw DriverFailedException("Cannot issue ioctl: driver " + driverName_ + " is not loaded");
    }
    return commandCode > 0;
}

bool DeviceDriver::isSupportedVersion(const std::string& minVersion) const {
    return version_ >= minVersion;
}

std::string DeviceDriver::describe() const {
    return driverName_ + " v" + version_ + " [" + deviceCategory_ + "] " +
           (loaded_ ? "(Loaded)" : "(Unloaded)");
}
