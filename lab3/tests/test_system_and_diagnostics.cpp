#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "Benchmark.hpp"
#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Computer.hpp"
#include "ComputerTask.hpp"
#include "Cpu.hpp"
#include "Exceptions.hpp"
#include "GraphicsCard.hpp"
#include "HardwareDiagnostic.hpp"
#include "HardwareSensor.hpp"
#include "IpAddress.hpp"
#include "Motherboard.hpp"
#include "PowerSupplyUnit.hpp"
#include "RamStick.hpp"
#include "StorageDrive.hpp"
#include "SystemLogEntry.hpp"
#include "SystemLogger.hpp"

TEST_CASE("SystemLogEntry and SystemLogger filtering and limits", "[logging]") {
    SystemLogEntry e1(100, "INFO", "KERNEL", "Kernel initialized");
    CHECK(e1.timestampSec() == 100);
    CHECK(e1.level() == "INFO");
    CHECK(e1.subsystem() == "KERNEL");
    CHECK(e1.message() == "Kernel initialized");
    CHECK_FALSE(e1.isError());
    CHECK(e1.matchesSubsystem("KERNEL"));
    CHECK_FALSE(e1.matchesSubsystem("GPU"));
    CHECK_FALSE(e1.format().empty());

    SystemLogEntry e2(101, "ERROR", "STORAGE", "Disk I/O error");
    CHECK(e2.isError());

    SystemLogger logger(3);
    logger.log("INFO", "SYS", "Boot start", 1);
    logger.log("WARN", "FAN", "High RPM", 2);
    logger.log("ERROR", "PSU", "Overload", 3);
    CHECK(logger.totalEntries() == 3);
    CHECK(logger.errorCount() == 1);

    auto errors = logger.getEntriesByLevel("ERROR");
    CHECK(errors.size() == 1);

    auto sysLogs = logger.getEntriesBySubsystem("SYS");
    CHECK(sysLogs.size() == 1);

    // Buffer overflow replaces oldest entry
    logger.log("INFO", "NET", "Link up", 4);
    CHECK(logger.totalEntries() == 3);

    logger.clear();
    CHECK(logger.totalEntries() == 0);

    CHECK_THROWS_AS(SystemLogger(0), InvalidConfigurationException);
    CHECK_THROWS_AS(SystemLogEntry(1, "", "SYS", "msg"), InvalidConfigurationException);
}

TEST_CASE("Benchmark testing CPU, GPU and storage", "[benchmark]") {
    Benchmark bench("Test Suite");
    CHECK(bench.benchmarkName() == "Test Suite");
    CHECK(bench.cpuScore() == 0);
    CHECK(bench.gpuScore() == 0);
    CHECK(bench.storageScore() == 0);
    CHECK_FALSE(bench.isPassing());

    auto cpu = support::makeCpu();
    bench.runCpuBenchmark(*cpu);
    CHECK(bench.cpuScore() > 0);

    Gpu gpu("GeForce RTX 4080", 9728, ClockFrequency::fromMHz(2505), 320);
    bench.runGpuBenchmark(gpu);
    CHECK(bench.gpuScore() > 0);

    StorageDrive drive("Fast NVMe", "NV1", "NVMe", ByteSize::fromGiB(500), 5000, 4500);
    bench.runStorageBenchmark(drive);
    CHECK(bench.storageScore() > 0);

    CHECK(bench.isPassing());
    CHECK(bench.totalCompositeScore() > 0);
    CHECK(bench.durationSeconds() > 0.0);
    CHECK_FALSE(bench.summary().empty());

    CHECK_THROWS_AS(Benchmark(""), InvalidConfigurationException);
}

TEST_CASE("HardwareDiagnostic reporting and alarms", "[hardware_diagnostic]") {
    HardwareDiagnostic diag;
    CHECK(diag.sensors().size() >= 3);
    CHECK(diag.allPassed());

    auto cpu = support::makeCpu();
    auto mobo = support::makeMotherboard();
    PowerSupplyUnit psu("Corsair", 750);
    mobo->connectPower(psu);

    diag.runDiagnostic(*cpu, *mobo, psu);
    CHECK(diag.allPassed());
    CHECK(diag.issuesCount() == 0);
    CHECK_FALSE(diag.hasCriticalIssues());
    CHECK_FALSE(diag.generateReport().empty());

    // Introduce an issue: disconnected motherboard power
    mobo->disconnectPower();
    diag.runDiagnostic(*cpu, *mobo, psu);
    CHECK_FALSE(diag.allPassed());
    CHECK(diag.issuesCount() > 0);
    CHECK(diag.hasCriticalIssues());
    CHECK(diag.generateReport().find("ISSUES DETECTED") != std::string::npos);

    // Overheated CPU in diagnostic
    cpu->thermalZone().setTemperature(88.0);
    diag.runDiagnostic(*cpu, *mobo, psu);
    CHECK(diag.issuesCount() >= 2);
    cpu->thermalZone().setTemperature(40.0);

    // Overloaded PSU in diagnostic
    mobo->connectPower(psu);
    PowerSupplyUnit overloadedPsu("Weak", 100);
    overloadedPsu.drawPower(100);
    CHECK(overloadedPsu.isOverloaded());
    CHECK(overloadedPsu.remainingCapacityWatt() == 0);
    CHECK_THROWS_AS(overloadedPsu.drawPower(10), PowerSupplyOverloadException);

    HardwareDiagnostic diag2;
    diag2.runDiagnostic(*cpu, *mobo, overloadedPsu);
    CHECK_FALSE(diag2.allPassed());

    // Sensor alarm
    HardwareSensor alarmSensor("Crit Sensor", "Temperature", 105.0, 95.0);
    diag.addSensor(alarmSensor);
    diag.runDiagnostic(*cpu, *mobo, psu);
    CHECK(diag.issuesCount() >= 1);
}

TEST_CASE("Computer edge cases and error handling", "[computer_edge]") {
    CHECK_THROWS_AS(Computer("", support::makeMotherboard(), support::makeCpu(), support::makePsu()),
                    InvalidConfigurationException);
    CHECK_THROWS_AS(Computer("NoMobo", nullptr, support::makeCpu(), support::makePsu()),
                    ComponentNotFoundException);

    auto pc = support::makeComputer();
    CHECK_THROWS_AS(pc.setGraphicsCard(nullptr), ComponentNotFoundException);
    CHECK_THROWS_AS(pc.addStorageDrive(nullptr), ComponentNotFoundException);

    // Power on without RAM
    auto emptyMobo = support::makeMotherboard();
    auto cpu = support::makeCpu();
    Computer pcNoRam("NoRamPC", emptyMobo, cpu, support::makePsu());
    CHECK_THROWS_AS(pcNoRam.powerOn(), BiosPostFailedException);
    CHECK(pcNoRam.powerState() == "ERROR");

    // Double powerOn and powerOff
    pc.powerOn();
    CHECK_NOTHROW(pc.powerOn()); // no-op when already ON
    pc.powerOff();
    CHECK_NOTHROW(pc.powerOff()); // no-op when already OFF

    // Run benchmark when off
    CHECK_THROWS_AS(pc.runBenchmark("OffBench"), DeviceNotConnectedException);

    // Workload memory exceeding RAM
    pc.powerOn();
    ComputerTask hugeTask(99, "Huge Workload", 1000, ByteSize::fromGiB(1000));
    CHECK_THROWS_AS(pc.executeWorkload(hugeTask), InsufficientMemoryException);
}

TEST_CASE("ComputerTask execution and completion percentage", "[computer_task]") {
    ComputerTask task(1, "Video Render", 1000, ByteSize::fromMiB(100));
    CHECK(task.jobId() == 1);
    CHECK(task.name() == "Video Render");
    CHECK(task.requiredCycles() == 1000);
    CHECK(task.completedCycles() == 0);
    CHECK_FALSE(task.isComplete());
    CHECK(task.completionPercentage() == 0.0);

    task.progressStep(500);
    CHECK(task.completedCycles() == 500);
    CHECK(task.completionPercentage() == 50.0);
    CHECK_FALSE(task.isComplete());

    task.progressStep(600); // reaches and caps at 1000
    CHECK(task.isComplete());
    CHECK(task.completionPercentage() == 100.0);

    task.reset();
    CHECK_FALSE(task.isComplete());
    CHECK(task.completedCycles() == 0);

    CHECK_FALSE(task.describe().empty());
    CHECK_THROWS_AS(ComputerTask(2, "", 100, ByteSize(10)), InvalidConfigurationException);
}

TEST_CASE("Computer end-to-end integration, boot, peripherals and benchmarks", "[computer_system]") {
    Computer pc = support::makeComputer();
    CHECK(pc.name() == "TestRig");
    CHECK(pc.powerState() == "OFF");
    CHECK_FALSE(pc.isOperational());

    CHECK_THROWS_AS(pc.executeWorkload(*(std::unique_ptr<ComputerTask>(
        new ComputerTask(1, "T1", 100, ByteSize(100))))), DeviceNotConnectedException);

    pc.powerOn();
    CHECK(pc.powerState() == "ON");
    CHECK(pc.isOperational());

    // Power consumption
    CHECK(pc.calculateTotalPowerConsumption() > 0);

    // Diagnostics while on
    auto diag = pc.runDiagnostics();
    CHECK(diag.allPassed());

    // Diagnostic warning branch
    pc.cpu()->thermalZone().setTemperature(88.0);
    auto warnDiag = pc.runDiagnostics();
    CHECK_FALSE(warnDiag.allPassed());
    pc.cpu()->thermalZone().setTemperature(40.0);

    // Benchmark
    auto bench = pc.runBenchmark("Standard Bench");
    CHECK(bench.isPassing());

    // PC without GPU and without drives
    Computer pcMinimal("MinimalPC", support::makeMotherboard(), support::makeCpu(), support::makePsu(500));
    pcMinimal.ram().installStick(RamStick("SN-M1", ByteSize::fromGiB(8), ClockFrequency::fromMHz(4800), "DDR5"));
    pcMinimal.powerOn();
    auto minBench = pcMinimal.runBenchmark("MinBench");
    CHECK(minBench.gpuScore() == 0);
    CHECK(pcMinimal.systemSummary().find("Integrated / None") != std::string::npos);

    // Workload
    ComputerTask task(10, "Shader Compilation", 5000000ULL, ByteSize::fromMiB(64));
    const double execTime = pc.executeWorkload(task);
    CHECK(execTime > 0.0);
    CHECK(task.isComplete());

    // USB operations
    pc.plugUsbDevice(1, "Webcam", 300);
    pc.unplugUsbDevice(1);

    // Network
    pc.connectNetwork(IpAddress({192, 168, 1, 200}, 24));
    CHECK(pc.networkAdapter().isLinkUp());
    pc.disconnectNetwork();
    CHECK_FALSE(pc.networkAdapter().isLinkUp());

    // Reboot
    pc.reboot();
    CHECK(pc.powerState() == "ON");

    // Summary
    const std::string summary = pc.systemSummary();
    CHECK(summary.find("TestRig") != std::string::npos);

    // Install OS
    pc.installOperatingSystem("Debian 12", "6.1.0");
    CHECK(pc.os().osName() == "Debian 12");

    pc.powerOff();
    CHECK(pc.powerState() == "OFF");
}
