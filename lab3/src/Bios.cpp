#include "Bios.hpp"

#include <utility>
#include "Exceptions.hpp"

Bios::Bios(std::string version, std::string vendor, bool uefiEnabled, bool secureBoot)
    : version_(std::move(version)), vendor_(std::move(vendor)),
      uefiEnabled_(uefiEnabled), secureBoot_(secureBoot),
      bootOrder_({"NVMe SSD", "SATA SSD", "USB Drive", "Network PXE"}) {
    if (version_.empty() || vendor_.empty()) {
        throw InvalidConfigurationException("BIOS version and vendor cannot be empty");
    }
}

void Bios::updateFirmware(std::string newVersion) {
    if (newVersion.empty()) {
        throw InvalidConfigurationException("New BIOS version cannot be empty");
    }
    version_ = std::move(newVersion);
}

void Bios::setSecureBoot(bool enable) {
    secureBoot_ = enable;
}

void Bios::setBootOrder(std::vector<std::string> order) {
    if (order.empty()) {
        throw InvalidConfigurationException("Boot order cannot be empty");
    }
    bootOrder_ = std::move(order);
}

const std::string& Bios::firstBootDevice() const {
    if (bootOrder_.empty()) {
        throw ComponentNotFoundException("Boot order is empty");
    }
    return bootOrder_.front();
}

bool Bios::performPost(bool hasCpu, bool hasRam) const {
    if (!hasCpu) {
        throw BiosPostFailedException("POST failed: CPU not detected in socket");
    }
    if (!hasRam) {
        throw BiosPostFailedException("POST failed: RAM not detected in memory slots");
    }
    return true;
}
