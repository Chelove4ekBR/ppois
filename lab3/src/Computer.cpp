#include "Computer.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kBaseMotherboardPower = 50;
constexpr uint32_t kRamStickPower = 5;
constexpr uint32_t kDrivePower = 10;
}

Computer::Computer(std::string name, MotherboardPtr mobo, CpuPtr cpu, PowerSupplyUnit psu)
    : name_(std::move(name)), motherboard_(std::move(mobo)),
      cpu_(std::move(cpu)), ramModule_(4), graphicsCard_(nullptr),
      psu_(std::move(psu)), computerCase_(), networkAdapter_(),
      soundCard_(), usbHub_(), os_(), logger_(), powerState_("OFF") {
    if (name_.empty()) {
        throw InvalidConfigurationException("Computer name cannot be empty");
    }
    if (!motherboard_) {
        throw ComponentNotFoundException("Motherboard cannot be null");
    }
    if (cpu_) {
        motherboard_->installCpu(cpu_);
    }
    motherboard_->connectPower(psu_);
    logger_.log("INFO", "SYS", "Computer " + name_ + " assembled successfully");
}

void Computer::setGraphicsCard(GraphicsCardPtr gpuCard) {
    if (!gpuCard) {
        throw ComponentNotFoundException("Cannot set null graphics card");
    }
    graphicsCard_ = std::move(gpuCard);
    motherboard_->installPcieDevice(graphicsCard_->gpu().model(), graphicsCard_->pcieLanesNeeded());
    logger_.log("INFO", "GPU", "Graphics card installed: " + graphicsCard_->gpu().model());
}

void Computer::powerOn() {
    if (powerState_ == "ON") {
        return;
    }
    powerState_ = "POST";
    logger_.log("INFO", "POWER", "Power button pressed, executing BIOS POST sequence");

    const bool hasRam = (ramModule_.installedCount() > 0);
    try {
        if (!motherboard_->isReadyToBoot(hasRam)) {
            powerState_ = "ERROR";
            logger_.log("ERROR", "BIOS", "POST failed: missing hardware or power fault");
            throw BiosPostFailedException("Computer cannot boot: POST failed");
        }
    } catch (const BiosPostFailedException&) {
        powerState_ = "ERROR";
        logger_.log("ERROR", "BIOS", "POST failed: missing hardware or power fault");
        throw;
    }

    psu_.drawPower(calculateTotalPowerConsumption());
    os_.boot();
    powerState_ = "ON";
    logger_.log("INFO", "OS", "Operating system " + os_.osName() + " booted successfully");
}

void Computer::powerOff() {
    if (powerState_ == "OFF") {
        return;
    }
    os_.shutdown();
    psu_.releasePower(psu_.currentDrawWatt());
    powerState_ = "OFF";
    logger_.log("INFO", "POWER", "System powered off cleanly");
}

void Computer::reboot() {
    logger_.log("INFO", "POWER", "Initiating system reboot");
    powerOff();
    powerOn();
}

bool Computer::isOperational() const {
    return powerState_ == "ON" && !cpu_->isOverheating() && (logger_.errorCount() == 0);
}

void Computer::installOperatingSystem(std::string osName, std::string kernelVer) {
    if (powerState_ == "ON") {
        powerOff();
    }
    os_ = OperatingSystem(std::move(osName), std::move(kernelVer));
    logger_.log("INFO", "INSTALL", "Installed new OS: " + os_.osName());
}

void Computer::addStorageDrive(StorageDrivePtr drive) {
    if (!drive) {
        throw ComponentNotFoundException("Cannot add null drive to computer");
    }
    drives_.push_back(std::move(drive));
    logger_.log("INFO", "STORAGE", "Added storage drive: " + drives_.back()->model());
}

void Computer::plugUsbDevice(uint32_t portNumber, const std::string& device, uint32_t currentMa) {
    usbHub_.plugDevice(portNumber, device, currentMa);
    logger_.log("INFO", "USB", "Plugged device '" + device + "' into port #" + std::to_string(portNumber));
}

void Computer::unplugUsbDevice(uint32_t portNumber) {
    usbHub_.unplugDevice(portNumber);
    logger_.log("INFO", "USB", "Unplugged device from port #" + std::to_string(portNumber));
}

void Computer::connectNetwork(const IpAddress& ip) {
    networkAdapter_.setIpAddress(ip);
    networkAdapter_.connectLink();
    logger_.log("INFO", "NET", "Network adapter connected with IP " + ip.toString());
}

void Computer::disconnectNetwork() {
    networkAdapter_.disconnectLink();
    logger_.log("INFO", "NET", "Network adapter disconnected");
}

HardwareDiagnostic Computer::runDiagnostics() {
    HardwareDiagnostic diag;
    diag.runDiagnostic(*cpu_, *motherboard_, psu_);
    if (!diag.allPassed()) {
        logger_.log("WARN", "DIAG", "Diagnostics detected issues: " + diag.generateReport());
    } else {
        logger_.log("INFO", "DIAG", "Hardware diagnostics passed successfully");
    }
    return diag;
}

Benchmark Computer::runBenchmark(const std::string& benchName) {
    if (powerState_ != "ON") {
        throw DeviceNotConnectedException("Cannot run benchmark while system is off");
    }
    Benchmark bench(benchName);
    bench.runCpuBenchmark(*cpu_);
    if (graphicsCard_) {
        bench.runGpuBenchmark(graphicsCard_->gpu());
    }
    if (!drives_.empty()) {
        bench.runStorageBenchmark(*drives_.front());
    }
    logger_.log("INFO", "BENCH", "Benchmark completed: " + bench.summary());
    return bench;
}

double Computer::executeWorkload(ComputerTask& task) {
    if (powerState_ != "ON") {
        throw DeviceNotConnectedException("Cannot execute workload: computer is powered off");
    }
    if (task.memoryNeeded().isGreaterThan(ramModule_.totalCapacity())) {
        throw InsufficientMemoryException("Workload requires " + task.memoryNeeded().toString() +
                                          " but RAM is only " + ramModule_.totalCapacity().toString());
    }
    logger_.log("INFO", "WORKLOAD", "Starting workload: " + task.name());
    const double executionTime = cpu_->executeTask(task.requiredCycles());
    task.progressStep(task.requiredCycles());
    logger_.log("INFO", "WORKLOAD", "Finished workload: " + task.name() + " in " +
                                     std::to_string(executionTime) + "s");
    return executionTime;
}

uint32_t Computer::calculateTotalPowerConsumption() const {
    uint32_t total = kBaseMotherboardPower;
    if (cpu_) {
        total += cpu_->tdpWatt();
    }
    if (graphicsCard_) {
        total += graphicsCard_->gpu().tdpWatt();
    }
    total += ramModule_.installedCount() * kRamStickPower;
    total += static_cast<uint32_t>(drives_.size()) * kDrivePower;
    return total;
}

std::string Computer::systemSummary() const {
    std::string summary = "=== Computer: " + name_ + " [" + powerState_ + "] ===\n";
    summary += "Motherboard: " + motherboard_->model() + " (" + motherboard_->chipset().model() + ")\n";
    summary += "CPU: " + (cpu_ ? (cpu_->model() + " (" + std::to_string(cpu_->totalCores()) + " cores)") : "None") + "\n";
    summary += "RAM: " + ramModule_.totalCapacity().toString() + " (" + std::to_string(ramModule_.installedCount()) + " sticks)\n";
    summary += "GPU: " + (graphicsCard_ ? graphicsCard_->gpu().model() : "Integrated / None") + "\n";
    summary += "Storage: " + std::to_string(drives_.size()) + " drive(s)\n";
    summary += "PSU: " + psu_.model() + " (" + std::to_string(psu_.ratedWattage()) + " W)\n";
    summary += "OS: " + os_.osName() + "\n";
    summary += "Power Draw: " + std::to_string(calculateTotalPowerConsumption()) + " W / " +
               std::to_string(psu_.ratedWattage()) + " W\n";
    return summary;
}
