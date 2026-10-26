#include "OperatingSystem.hpp"

#include <utility>
#include "Exceptions.hpp"

OperatingSystem::OperatingSystem(std::string osName, std::string kernelVersion, bool is64Bit)
    : osName_(std::move(osName)), kernelVersion_(std::move(kernelVersion)),
      is64Bit_(is64Bit), booted_(false), scheduler_(), drivers_(), vmm_() {
    if (osName_.empty() || kernelVersion_.empty()) {
        throw InvalidConfigurationException("Operating system name and kernel version cannot be empty");
    }
}

void OperatingSystem::boot() {
    booted_ = true;
    for (auto& drv : drivers_) {
        drv.load();
    }
    // Запуск системного процесса init / systemd
    scheduler_.createProcess("systemd", ByteSize::fromMiB(32), 1);
}

void OperatingSystem::shutdown() {
    for (const auto& proc : scheduler_.processes()) {
        if (!proc->isTerminated()) {
            proc->terminate();
        }
    }
    for (auto& drv : drivers_) {
        drv.unload();
    }
    booted_ = false;
}

void OperatingSystem::installDriver(DeviceDriver drv) {
    if (booted_) {
        drv.load();
    }
    drivers_.push_back(std::move(drv));
}

ProcessPtr OperatingSystem::launchApplication(const std::string& name, ByteSize memory, uint32_t priority) {
    if (!booted_) {
        throw DeviceNotConnectedException("Cannot launch application: OS is not booted");
    }
    vmm_.allocateSegment(memory, true, false);
    return scheduler_.createProcess(name, memory, priority);
}

void OperatingSystem::terminateProcess(uint32_t pid) {
    if (!booted_) {
        throw DeviceNotConnectedException("Cannot terminate process: OS is not running");
    }
    scheduler_.killProcess(pid);
}

uint32_t OperatingSystem::installedDriverCount() const {
    return static_cast<uint32_t>(drivers_.size());
}
