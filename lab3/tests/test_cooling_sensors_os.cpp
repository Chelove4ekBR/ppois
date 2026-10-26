#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "ByteSize.hpp"
#include "CaseFan.hpp"
#include "ComputerCase.hpp"
#include "DeviceDriver.hpp"
#include "Exceptions.hpp"
#include "HardwareSensor.hpp"
#include "OperatingSystem.hpp"
#include "Process.hpp"
#include "ProcessScheduler.hpp"
#include "UninterruptiblePowerSupply.hpp"

TEST_CASE("CaseFan PWM speed, airflow and noise", "[case_fan]") {
    CaseFan fan(140, 2000, 80.0);
    CHECK(fan.diameterMm() == 140);
    CHECK(fan.maxRpm() == 2000);
    CHECK(fan.maxAirflowCfm() == 80.0);
    CHECK(fan.pwmDuty() == 50);
    CHECK(fan.isSpinning());

    fan.setPwmDuty(100);
    CHECK(fan.currentRpm() == 2000);
    CHECK(fan.currentAirflowCfm() == 80.0);
    CHECK(fan.noiseLevelDb() > 30.0);

    fan.stop();
    CHECK(fan.currentRpm() == 0);
    CHECK_FALSE(fan.isSpinning());
    CHECK(fan.noiseLevelDb() == 0.0);

    CHECK_THROWS_AS(fan.setPwmDuty(110), InvalidConfigurationException);
    CHECK_THROWS_AS(CaseFan(0, 1000, 50.0), InvalidConfigurationException);
}

TEST_CASE("ComputerCase clearance and fan airflow aggregation", "[computer_case]") {
    ComputerCase ccase("NZXT H7 Flow", "ATX", 4, 400);
    CHECK(ccase.model() == "NZXT H7 Flow");
    CHECK(ccase.formFactorSupported() == "ATX");
    CHECK(ccase.driveBaysCount() == 4);
    CHECK(ccase.maxGpuLengthMm() == 400);

    CHECK(ccase.canFitGpu(350));
    CHECK_FALSE(ccase.canFitGpu(450));

    CHECK(ccase.totalAirflowCfm() > 0.0);
    CHECK(ccase.averageNoiseDb() > 0.0);

    ccase.setAllFansPwm(80);
    ccase.installFan(CaseFan(120, 1500, 50.0));
    CHECK(ccase.fans().size() == 3);

    CHECK_THROWS_AS(ComputerCase("", "ATX", 2, 300), InvalidConfigurationException);
}

TEST_CASE("UninterruptiblePowerSupply outage and battery depletion", "[ups]") {
    UninterruptiblePowerSupply ups(1500, 500, 100);
    CHECK(ups.capacityVa() == 1500);
    CHECK(ups.batteryPercent() == 100);
    CHECK_FALSE(ups.isOnBattery());
    CHECK(ups.estimatedRuntimeMinutes() > 10.0);

    ups.simulatePowerOutage();
    CHECK(ups.isOnBattery());

    ups.discharge(10);
    CHECK(ups.batteryPercent() < 100);

    ups.restoreMainsPower();
    CHECK_FALSE(ups.isOnBattery());

    ups.discharge(10); // no-op when not on battery
    CHECK_FALSE(ups.isOnBattery());

    ups.chargeBattery(20);
    CHECK(ups.batteryPercent() == 100); // capped at 100

    ups.chargeBattery(100);
    CHECK(ups.batteryPercent() == 100);

    UninterruptiblePowerSupply zeroLoad(1000, 0, 100);
    CHECK(zeroLoad.estimatedRuntimeMinutes() == 999.0);
    CHECK_THROWS_AS(UninterruptiblePowerSupply(0, 100), InvalidConfigurationException);
    CHECK_THROWS_AS(UninterruptiblePowerSupply(1000, 100, 105), InvalidConfigurationException);

    ups.simulatePowerOutage();
    CHECK_THROWS_AS(ups.discharge(60), BatteryDepletedException);
    CHECK(ups.batteryPercent() == 0);
}

TEST_CASE("HardwareSensor alarm thresholds and deviation", "[hardware_sensor]") {
    HardwareSensor sensor("VRM Temp", "Temperature", 65.0, 90.0);
    CHECK(sensor.sensorName() == "VRM Temp");
    CHECK(sensor.sensorType() == "Temperature");
    CHECK(sensor.currentValue() == 65.0);
    CHECK(sensor.thresholdValue() == 90.0);
    CHECK_FALSE(sensor.isAlarm());

    sensor.updateValue(95.0);
    CHECK(sensor.isAlarm());
    CHECK(sensor.deviationPercent() > 0.0);
    CHECK_FALSE(sensor.describe().empty());

    HardwareSensor zeroThresh("Zero", "Voltage", 5.0, 0.0);
    CHECK(zeroThresh.deviationPercent() == 0.0);

    CHECK_THROWS_AS(HardwareSensor("", "Temperature", 10.0, 50.0), InvalidConfigurationException);
}

TEST_CASE("Process lifecycle and resource tracking", "[process]") {
    Process proc(10, "code-editor", ByteSize::fromMiB(256), 5);
    CHECK(proc.pid() == 10);
    CHECK(proc.name() == "code-editor");
    CHECK(proc.memoryUsage().toMiB() == 256.0);
    CHECK(proc.priority() == 5);
    CHECK(proc.state() == "RUNNING");
    CHECK_FALSE(proc.isTerminated());

    proc.setCpuUsage(35.0);
    CHECK(proc.cpuUsagePercent() == 35.0);

    proc.allocateMemory(ByteSize::fromMiB(64));
    CHECK(proc.memoryUsage().toMiB() == 320.0);

    proc.freeMemory(ByteSize::fromMiB(100));
    CHECK(proc.memoryUsage().toMiB() == 220.0);

    proc.freeMemory(ByteSize::fromGiB(100));
    CHECK(proc.memoryUsage().bytes() == 0);

    proc.suspend();
    CHECK(proc.state() == "SLEEPING");
    CHECK(proc.cpuUsagePercent() == 0.0);
    proc.setCpuUsage(50.0);
    CHECK(proc.cpuUsagePercent() == 0.0);

    proc.resume();
    CHECK(proc.state() == "RUNNING");

    proc.terminate();
    CHECK(proc.isTerminated());
    CHECK(proc.memoryUsage().bytes() == 0);

    CHECK_THROWS_AS(proc.allocateMemory(ByteSize::fromMiB(10)), ProcessNotFoundException);
    CHECK_THROWS_AS(proc.suspend(), ProcessNotFoundException);
    CHECK_THROWS_AS(proc.resume(), ProcessNotFoundException);
    CHECK_THROWS_AS(Process(1, "", ByteSize(100)), InvalidConfigurationException);
    CHECK_THROWS_AS(Process(1, "bad-pri", ByteSize(100), 0), InvalidConfigurationException);
}

TEST_CASE("ProcessScheduler scheduling cycles and queue management", "[scheduler]") {
    ProcessScheduler sched("RoundRobin");
    CHECK(sched.algorithm() == "RoundRobin");
    CHECK(sched.activeProcessesCount() == 0);

    auto p1 = sched.createProcess("compiler", ByteSize::fromMiB(512), 3);
    auto p2 = sched.createProcess("browser", ByteSize::fromMiB(1024), 8);
    CHECK(sched.processes().size() == 2);
    CHECK(sched.activeProcessesCount() == 2);
    CHECK(sched.totalMemoryUsed().toMiB() == 1536.0);

    const double avgLoad = sched.scheduleCycle();
    CHECK(avgLoad > 0.0);
    CHECK(sched.totalCpuLoad() > 0.0);

    auto found = sched.findProcess(p1->pid());
    CHECK(found->name() == "compiler");

    sched.killProcess(p1->pid());
    CHECK(p1->isTerminated());
    CHECK(sched.activeProcessesCount() == 1);

    CHECK_THROWS_AS(sched.findProcess(999), ProcessNotFoundException);
    CHECK_THROWS_AS(ProcessScheduler(""), InvalidConfigurationException);
}

TEST_CASE("DeviceDriver loading and ioctl commands", "[driver]") {
    DeviceDriver drv("amdgpu.ko", "GPU", "23.20");
    CHECK(drv.driverName() == "amdgpu.ko");
    CHECK(drv.deviceCategory() == "GPU");
    CHECK(drv.version() == "23.20");
    CHECK_FALSE(drv.isLoaded());

    CHECK_THROWS_AS(drv.sendIoctl(1), DriverFailedException);

    drv.load();
    CHECK(drv.isLoaded());
    CHECK(drv.sendIoctl(0x10));
    CHECK_FALSE(drv.sendIoctl(0));

    CHECK(drv.isSupportedVersion("22.00"));
    CHECK_FALSE(drv.isSupportedVersion("24.00"));
    CHECK_FALSE(drv.describe().empty());

    drv.unload();
    CHECK_FALSE(drv.isLoaded());

    CHECK_THROWS_AS(DeviceDriver("", "GPU", "1.0"), InvalidConfigurationException);
}

TEST_CASE("OperatingSystem boot, applications and drivers", "[os]") {
    OperatingSystem os("Fedora Linux", "6.7.0", true);
    CHECK(os.osName() == "Fedora Linux");
    CHECK(os.kernelVersion() == "6.7.0");
    CHECK(os.is64Bit());
    CHECK_FALSE(os.isSystemBooted());

    os.installDriver(DeviceDriver("nvme.ko", "Storage", "1.0"));
    CHECK(os.installedDriverCount() == 1);

    os.boot();
    CHECK(os.isSystemBooted());
    CHECK(os.drivers()[0].isLoaded());
    CHECK(os.scheduler().activeProcessesCount() >= 1); // systemd launched

    auto app = os.launchApplication("game", ByteSize::fromGiB(2), 2);
    CHECK(app->name() == "game");

    os.terminateProcess(app->pid());
    CHECK(app->isTerminated());

    os.installDriver(DeviceDriver("extra.ko", "Net", "1.0"));
    CHECK(os.installedDriverCount() == 2);

    os.shutdown();
    CHECK_FALSE(os.isSystemBooted());
    CHECK_FALSE(os.drivers()[0].isLoaded());

    CHECK_THROWS_AS(os.launchApplication("app", ByteSize::fromMiB(10)), DeviceNotConnectedException);
    CHECK_THROWS_AS(os.terminateProcess(1), DeviceNotConnectedException);
    CHECK_THROWS_AS(OperatingSystem("", "6.7.0"), InvalidConfigurationException);
    CHECK_THROWS_AS(OperatingSystem("Linux", ""), InvalidConfigurationException);
}
