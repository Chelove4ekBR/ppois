#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "Bios.hpp"
#include "Chipset.hpp"
#include "ClockFrequency.hpp"
#include "Cpu.hpp"
#include "CpuCooler.hpp"
#include "CpuCore.hpp"
#include "Exceptions.hpp"
#include "Motherboard.hpp"
#include "PcieBus.hpp"
#include "PcieSlot.hpp"
#include "ThermalPaste.hpp"

TEST_CASE("CpuCore execution, throttling and activity toggle", "[cpu_core]") {
    CpuCore core(0, ClockFrequency::fromGHz(3.0));
    CHECK(core.coreId() == 0);
    CHECK(core.isActive());
    CHECK(core.usagePercent() == 0.0);
    CHECK(core.currentFrequency().toGHz() == 3.0);

    core.setUsage(75.5);
    CHECK(core.usagePercent() == 75.5);
    CHECK_THROWS_AS(core.setUsage(150.0), InvalidConfigurationException);
    CHECK_THROWS_AS(core.setUsage(-5.0), InvalidConfigurationException);

    core.throttle(0.5);
    CHECK(core.currentFrequency().toGHz() == 1.5);
    CHECK_THROWS_AS(core.throttle(0.05), InvalidConfigurationException);
    CHECK_THROWS_AS(core.throttle(1.5), InvalidConfigurationException);

    core.resetFrequency();
    CHECK(core.currentFrequency().toGHz() == 3.0);

    const double execTime = core.executeCycles(3000000000ULL);
    CHECK(execTime > 0.99);

    core.setActive(false);
    CHECK_FALSE(core.isActive());
    CHECK(core.usagePercent() == 0.0);
    CHECK_THROWS_AS(core.setUsage(50.0), InvalidConfigurationException);
    CHECK_THROWS_AS(core.executeCycles(100), ComponentNotFoundException);
}

TEST_CASE("CpuCooler modes and RPM scaling", "[cpu_cooler]") {
    CpuCooler cooler("Liquid", 250, 2200);
    CHECK(cooler.coolerType() == "Liquid");
    CHECK(cooler.maxTdpWatt() == 250);
    CHECK(cooler.maxFanRpm() == 2200);
    CHECK(cooler.isPumpActive());
    CHECK(cooler.isSufficientFor(200));
    CHECK_FALSE(cooler.isSufficientFor(300));

    cooler.setFanSpeed(1100);
    CHECK(cooler.fanRpm() == 1100);
    CHECK(cooler.rpmPercentage() == 50.0);
    CHECK_THROWS_AS(cooler.setFanSpeed(3000), InvalidConfigurationException);

    const double delta = cooler.calculateCoolingDelta(70.0, 150);
    CHECK(delta > 0.0);
    CHECK(cooler.calculateCoolingDelta(20.0, 150) == 0.0);

    cooler.stopFan();
    CHECK(cooler.fanRpm() == 0);

    CHECK_THROWS_AS(CpuCooler("Air", 0, 1000), InvalidConfigurationException);
}

TEST_CASE("ThermalPaste degradation and conductivity", "[thermal_paste]") {
    ThermalPaste paste("Noctua NT-H1", 8.9);
    CHECK(paste.brand() == "Noctua NT-H1");
    CHECK(paste.thermalConductivity() == 8.9);
    CHECK(paste.appliedHours() == 0);
    CHECK_FALSE(paste.isDegraded());
    CHECK(paste.effectiveConductivity() == 8.9);

    paste.age(10000);
    CHECK(paste.appliedHours() == 10000);
    CHECK(paste.isDegraded());
    CHECK(paste.effectiveConductivity() < 8.9);

    paste.replacePaste("Thermal Grizzly", 12.5);
    CHECK(paste.brand() == "Thermal Grizzly");
    CHECK(paste.appliedHours() == 0);
    CHECK_FALSE(paste.isDegraded());
    CHECK(paste.thermalConductivity() == 12.5);

    CHECK_THROWS_AS(ThermalPaste("Bad", -1.0), InvalidConfigurationException);
    CHECK_THROWS_AS(paste.replacePaste("Bad", 0.0), InvalidConfigurationException);
}

TEST_CASE("Cpu workload distribution and cooling", "[cpu]") {
    Cpu cpu("Core i7-14700K", "LGA1700", 4, ClockFrequency::fromGHz(3.5), ByteSize::fromMiB(33), 125);
    CHECK(cpu.totalCores() == 4);
    CHECK(cpu.activeCoresCount() == 4);
    CHECK(cpu.averageUsage() == 0.0);

    cpu.setCoreUsage(0, 50.0);
    cpu.setCoreUsage(1, 50.0);
    CHECK(cpu.averageUsage() == 25.0);
    CHECK_THROWS_AS(cpu.setCoreUsage(10, 50.0), ComponentNotFoundException);

    cpu.setCoreActive(0, false);
    cpu.setCoreActive(1, false);
    cpu.setCoreActive(2, false);
    cpu.setCoreActive(3, false);
    CHECK(cpu.activeCoresCount() == 0);
    CHECK(cpu.averageUsage() == 0.0);
    CHECK_THROWS_AS(cpu.executeTask(1000), ComponentNotFoundException);
    CHECK_THROWS_AS(cpu.setCoreActive(10, true), ComponentNotFoundException);

    cpu.setCoreActive(0, true);
    cpu.setCoreActive(1, true);
    cpu.setCoreActive(2, true);
    cpu.setCoreActive(3, true);

    auto cooler = std::make_shared<CpuCooler>("Air", 200, 2000);
    cpu.attachCooler(cooler);
    CHECK(cpu.cooler() != nullptr);
    CHECK_THROWS_AS(cpu.attachCooler(nullptr), ComponentNotFoundException);

    const double taskDuration = cpu.executeTask(10000000ULL);
    CHECK(taskDuration > 0.0);
    CHECK_FALSE(cpu.isOverheating());

    cpu.overclockAllCores(10.0);
    CHECK(cpu.cores()[0].baseFrequency().toGHz() > 3.5);

    CHECK_THROWS_AS(Cpu("Empty", "AM5", 0, ClockFrequency::fromGHz(3.0), ByteSize::fromMiB(16), 65),
                    InvalidConfigurationException);
}

TEST_CASE("Chipset socket support and PCIe limits", "[chipset]") {
    Chipset chipset("AMD X670E", "AM5", 44, true);
    CHECK(chipset.model() == "AMD X670E");
    CHECK(chipset.supportsSocket("AM5"));
    CHECK_FALSE(chipset.supportsSocket("LGA1700"));
    CHECK(chipset.validatePcieLanes(16));
    CHECK_FALSE(chipset.validatePcieLanes(64));
    CHECK(chipset.canOverclock());
    CHECK_FALSE(chipset.describe().empty());

    CHECK_THROWS_AS(Chipset("", "AM5"), InvalidConfigurationException);
}

TEST_CASE("PcieSlot bandwidth and plug operations", "[pcie_slot]") {
    PcieSlot slot1(1, 4, 16);
    CHECK(slot1.slotId() == 1);
    CHECK(slot1.laneCount() == 16);
    CHECK(slot1.bandwidthGbps() > 30.0);
    CHECK(slot1.isAvailable());
    CHECK(slot1.canAccommodateLanes(8));
    CHECK_FALSE(slot1.canAccommodateLanes(32));

    slot1.plugDevice("NVIDIA RTX 4080", 16);
    CHECK(slot1.isOccupied());
    CHECK_FALSE(slot1.isAvailable());
    CHECK(slot1.connectedDevice() == "NVIDIA RTX 4080");

    CHECK_THROWS_AS(slot1.plugDevice("SoundCard", 1), PortOccupiedException);

    slot1.unplugDevice();
    CHECK(slot1.isAvailable());
    CHECK_THROWS_AS(slot1.unplugDevice(), DeviceNotConnectedException);

    PcieSlot smallSlot(2, 3, 4);
    CHECK_THROWS_AS(smallSlot.plugDevice("GPU", 16), IncompatibleHardwareException);
    CHECK_THROWS_AS(PcieSlot(3, 0, 16), InvalidConfigurationException);
    PcieSlot gen5Slot(4, 5, 16);
    CHECK(gen5Slot.bandwidthGbps() > 0.0);
}

TEST_CASE("PcieBus lane allocation and slot discovery", "[pcie_bus]") {
    PcieBus bus(32);
    bus.addSlot(PcieSlot(1, 4, 16));
    bus.addSlot(PcieSlot(2, 4, 8));
    bus.addSlot(PcieSlot(3, 4, 4));

    CHECK(bus.slots().size() == 3);
    CHECK(bus.totalOccupiedLanes() == 0);
    CHECK(bus.hasAvailableSlot(16));
    CHECK_FALSE(bus.hasAvailableSlot(100));

    CHECK_THROWS_AS(bus.allocateSlot("ExceedLanes", 100), IncompatibleHardwareException);

    const uint32_t assigned1 = bus.allocateSlot("Graphics Card", 16);
    CHECK(assigned1 == 1);
    CHECK(bus.totalOccupiedLanes() == 16);

    const uint32_t assigned2 = bus.allocateSlot("Capture Card", 4);
    CHECK(assigned2 == 2); // fits into 8x slot
    CHECK(bus.totalOccupiedLanes() == 24);

    bus.releaseSlot(assigned1);
    CHECK(bus.totalOccupiedLanes() == 8);

    PcieBus smallBus(16);
    smallBus.addSlot(PcieSlot(1, 4, 4));
    CHECK_FALSE(smallBus.hasAvailableSlot(8));
    CHECK_THROWS_AS(smallBus.allocateSlot("BigGPU", 8), PortOccupiedException);

    PcieBus busWithSlot(16);
    busWithSlot.addSlot(PcieSlot(5, 4, 16));
    const PcieBus& constRef = busWithSlot;
    CHECK(constRef.findSlot(5).slotId() == 5);
    CHECK_THROWS_AS(constRef.findSlot(999), ComponentNotFoundException);

    const PcieBus constBus(16);
    CHECK_THROWS_AS(constBus.findSlot(1), ComponentNotFoundException);
    CHECK_THROWS_AS(bus.findSlot(99), ComponentNotFoundException);
    CHECK_THROWS_AS(PcieBus(0), InvalidConfigurationException);
}

TEST_CASE("Bios POST and boot priority", "[bios]") {
    Bios bios("1.2.0", "AMI", true, true);
    CHECK(bios.version() == "1.2.0");
    CHECK(bios.vendor() == "AMI");
    CHECK(bios.isUefiEnabled());
    CHECK(bios.isSecureBootEnabled());
    CHECK(bios.firstBootDevice() == "NVMe SSD");

    bios.updateFirmware("1.3.0");
    CHECK(bios.version() == "1.3.0");
    CHECK_THROWS_AS(bios.updateFirmware(""), InvalidConfigurationException);

    bios.setSecureBoot(false);
    CHECK_FALSE(bios.isSecureBootEnabled());

    bios.setBootOrder({"USB Drive", "NVMe SSD"});
    CHECK(bios.firstBootDevice() == "USB Drive");
    CHECK_THROWS_AS(bios.setBootOrder({}), InvalidConfigurationException);

    CHECK(bios.performPost(true, true));
    CHECK_THROWS_AS(bios.performPost(false, true), BiosPostFailedException);
    CHECK_THROWS_AS(bios.performPost(true, false), BiosPostFailedException);
    CHECK_THROWS_AS(Bios("", "AMI"), InvalidConfigurationException);
    CHECK_THROWS_AS(Bios("1.0", ""), InvalidConfigurationException);
}

TEST_CASE("Motherboard assembly, power and boot readiness", "[motherboard]") {
    auto mobo = support::makeMotherboard();
    CHECK(mobo->model() == "MSI B650 Tomahawk");
    CHECK(mobo->formFactor() == "ATX");
    CHECK(mobo->socket() == "AM5");
    CHECK_FALSE(mobo->isPowerConnected());

    auto cpu = support::makeCpu();
    mobo->installCpu(cpu);
    CHECK(mobo->cpu() != nullptr);

    // Incompatible socket
    auto intelCpu = std::make_shared<Cpu>("Intel i5", "LGA1700", 6, ClockFrequency::fromGHz(3.0), ByteSize::fromMiB(12), 65);
    CHECK_THROWS_AS(mobo->installCpu(intelCpu), IncompatibleHardwareException);
    CHECK_THROWS_AS(mobo->installCpu(nullptr), ComponentNotFoundException);

    PowerSupplyUnit psu("EVGA 650", 650);
    mobo->connectPower(psu);
    CHECK(mobo->isPowerConnected());

    CHECK_THROWS_AS(PowerSupplyUnit("ZeroPsu", 0), InvalidConfigurationException);

    Chipset badChipset("B650", "LGA1700", 36, true);
    Motherboard moboMismatch("MoboMismatch", "ATX", "AM5", badChipset, mobo->bios(), 4);
    CHECK_THROWS_AS(moboMismatch.installCpu(cpu), IncompatibleHardwareException);
    CHECK_THROWS_AS(Motherboard("BadRam", "ATX", "AM5", mobo->chipset(), mobo->bios(), 0), InvalidConfigurationException);

    CHECK(mobo->isReadyToBoot(true));
    CHECK_THROWS_AS(mobo->isReadyToBoot(false), BiosPostFailedException);

    mobo->disconnectPower();
    CHECK_FALSE(mobo->isReadyToBoot(true));

    uint32_t slotId = mobo->installPcieDevice("SoundCard", 4);
    CHECK(slotId > 0);

    auto removedCpu = mobo->removeCpu();
    CHECK(removedCpu != nullptr);
    CHECK(mobo->cpu() == nullptr);
    CHECK_THROWS_AS(mobo->removeCpu(), ComponentNotFoundException);
}
