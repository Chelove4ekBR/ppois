#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Exceptions.hpp"
#include "PowerSupplyUnit.hpp"
#include "ThermalZone.hpp"
#include "Voltage.hpp"

TEST_CASE("ByteSize conversions, arithmetic and errors", "[bytesize]") {
    ByteSize b1 = ByteSize::fromKiB(2);
    CHECK(b1.bytes() == 2048);
    CHECK(b1.toKiB() == 2.0);

    ByteSize b2 = ByteSize::fromMiB(1);
    CHECK(b2.bytes() == 1024 * 1024);
    CHECK(b2.toMiB() == 1.0);

    ByteSize b3 = ByteSize::fromGiB(4);
    CHECK(b3.toGiB() == 4.0);

    ByteSize sum = b1.plus(b2);
    CHECK(sum.bytes() == 2048 + 1024 * 1024);

    ByteSize diff = b2.minus(b1);
    CHECK(diff.bytes() == 1024 * 1024 - 2048);
    CHECK_THROWS_AS(b1.minus(b2), InvalidConfigurationException);

    ByteSize mult = b1.times(4);
    CHECK(mult.bytes() == 8192);

    CHECK(b2.isGreaterThan(b1));
    CHECK_FALSE(b1.isGreaterThan(b2));

    CHECK(b3.toString() == "4.00 GiB");
    CHECK(b2.toString() == "1.00 MiB");
    CHECK(b1.toString() == "2.00 KiB");
    CHECK(ByteSize(500).toString() == "500 B");
}

TEST_CASE("ClockFrequency conversions, overclocking and arithmetic", "[clockfrequency]") {
    ClockFrequency f1 = ClockFrequency::fromMHz(800);
    CHECK(f1.toMHz() == 800.0);
    CHECK(f1.hertz() == 800000000ULL);

    ClockFrequency f2 = ClockFrequency::fromGHz(3.6);
    CHECK(f2.toGHz() == 3.6);
    CHECK_THROWS_AS(ClockFrequency::fromGHz(-1.0), InvalidConfigurationException);

    ClockFrequency boosted = f2.overclock(10.0);
    CHECK(boosted.hertz() > f2.hertz());
    CHECK_THROWS_AS(f2.overclock(-110.0), InvalidConfigurationException);

    ClockFrequency sum = f1.plus(f2);
    CHECK(sum.hertz() == f1.hertz() + f2.hertz());

    ClockFrequency diff = f2.minus(f1);
    CHECK(diff.hertz() == f2.hertz() - f1.hertz());
    CHECK_THROWS_AS(f1.minus(f2), InvalidConfigurationException);

    CHECK(f2.isGreaterThan(f1));
    CHECK_FALSE(f1.isGreaterThan(f2));

    CHECK(f2.toString() == "3.60 GHz");
    CHECK(f1.toString() == "800.00 MHz");
    CHECK(ClockFrequency(500000).toString() == "500.00 kHz");
    CHECK(ClockFrequency(400).toString() == "400 Hz");
}

TEST_CASE("Voltage operations and range checks", "[voltage]") {
    Voltage v1(1200);
    CHECK(v1.millivolts() == 1200);
    CHECK(v1.toVolts() == 1.2);

    Voltage minSafe(1100);
    Voltage maxSafe(1350);
    CHECK(v1.isSafe(minSafe, maxSafe));
    CHECK_FALSE(Voltage(1000).isSafe(minSafe, maxSafe));
    CHECK_FALSE(Voltage(1400).isSafe(minSafe, maxSafe));

    Voltage steppedUp = v1.stepUp(50);
    CHECK(steppedUp.millivolts() == 1250);

    Voltage steppedDown = v1.stepDown(100);
    CHECK(steppedDown.millivolts() == 1100);
    CHECK_THROWS_AS(v1.stepDown(2000), InvalidConfigurationException);

    CHECK(v1.isGreaterThan(steppedDown));
    CHECK(v1.toString() == "1.200 V");
}

TEST_CASE("ThermalZone threshold validation, throttling and critical heat", "[thermalzone]") {
    ThermalZone tz(40.0, 80.0, 95.0);
    CHECK(tz.temperatureC() == 40.0);
    CHECK(tz.throttleTempC() == 80.0);
    CHECK(tz.criticalTempC() == 95.0);
    CHECK(tz.headroomC() == 55.0);
    CHECK_FALSE(tz.isThrottling());
    CHECK_FALSE(tz.isCritical());

    tz.updateTemperature(42.0);
    CHECK(tz.temperatureC() == 82.0);
    CHECK(tz.isThrottling());
    CHECK_FALSE(tz.isCritical());

    tz.cool(10.0);
    CHECK(tz.temperatureC() == 72.0);
    CHECK_FALSE(tz.isThrottling());
    CHECK_THROWS_AS(tz.cool(-5.0), InvalidConfigurationException);

    CHECK_THROWS_AS(tz.setTemperature(100.0), OverheatingException);
    CHECK_THROWS_AS(tz.setTemperature(-300.0), InvalidConfigurationException);

    CHECK_THROWS_AS(ThermalZone(30.0, 95.0, 90.0), InvalidConfigurationException);
    CHECK_THROWS_AS(ThermalZone(30.0, -60.0, 90.0), InvalidConfigurationException);
    CHECK_THROWS_AS(ThermalZone(100.0, 80.0, 95.0), OverheatingException);
}

TEST_CASE("PowerSupplyUnit wattage management and load efficiency", "[psu]") {
    PowerSupplyUnit psu("Corsair HX850", 850, "80 PLUS Platinum", true);
    CHECK(psu.model() == "Corsair HX850");
    CHECK(psu.ratedWattage() == 850);
    CHECK(psu.efficiencyRating() == "80 PLUS Platinum");
    CHECK(psu.isModular());
    CHECK(psu.currentDrawWatt() == 0);
    CHECK(psu.remainingCapacityWatt() == 850);
    CHECK_FALSE(psu.isOverloaded());

    psu.drawPower(400);
    CHECK(psu.currentDrawWatt() == 400);
    CHECK(psu.remainingCapacityWatt() == 450);
    CHECK(psu.loadPercentage() > 40.0);
    CHECK(psu.calculateEfficiency() == 92.0);

    psu.releasePower(100);
    CHECK(psu.currentDrawWatt() == 300);

    psu.releasePower(500); // excess release clamped to 0
    CHECK(psu.currentDrawWatt() == 0);

    psu.drawPower(100);
    CHECK(psu.calculateEfficiency() == 80.0);

    CHECK_THROWS_AS(psu.drawPower(1000), PowerSupplyOverloadException);
    CHECK_THROWS_AS(PowerSupplyUnit("Bad", 0), InvalidConfigurationException);
}
