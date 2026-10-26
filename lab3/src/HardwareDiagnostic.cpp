#include "HardwareDiagnostic.hpp"

#include "Exceptions.hpp"

HardwareDiagnostic::HardwareDiagnostic() : allPassed_(true) {
    sensors_.emplace_back("CPU Temperature", "Temperature", 42.0, 95.0);
    sensors_.emplace_back("+12V Rail", "Voltage", 12.05, 12.80);
    sensors_.emplace_back("CPU Fan", "Fan", 1200.0, 2500.0);
}

void HardwareDiagnostic::addSensor(const HardwareSensor& sensor) {
    sensors_.push_back(sensor);
}

void HardwareDiagnostic::runDiagnostic(const Cpu& cpu, const Motherboard& mobo, const PowerSupplyUnit& psu) {
    detectedIssues_.clear();
    allPassed_ = true;

    if (cpu.isOverheating()) {
        detectedIssues_.push_back("CPU temperature exceeds threshold: " +
                                  std::to_string(cpu.thermalZone().temperatureC()) + " C");
        allPassed_ = false;
    }
    if (!mobo.isPowerConnected()) {
        detectedIssues_.push_back("Motherboard main 24-pin ATX power cable disconnected");
        allPassed_ = false;
    }
    if (psu.isOverloaded()) {
        detectedIssues_.push_back("Power Supply is overloaded: drawing " +
                                  std::to_string(psu.currentDrawWatt()) + " W");
        allPassed_ = false;
    }
    for (const auto& sensor : sensors_) {
        if (sensor.isAlarm()) {
            detectedIssues_.push_back("Sensor alarm: " + sensor.describe());
            allPassed_ = false;
        }
    }
}

std::size_t HardwareDiagnostic::issuesCount() const {
    return detectedIssues_.size();
}

bool HardwareDiagnostic::hasCriticalIssues() const {
    return !allPassed_ && !detectedIssues_.empty();
}

std::string HardwareDiagnostic::generateReport() const {
    if (allPassed_) {
        return "Hardware Diagnostic Report: ALL SYSTEMS NOMINAL (0 issues detected)";
    }
    std::string report = "Hardware Diagnostic Report: ISSUES DETECTED (" +
                         std::to_string(detectedIssues_.size()) + " warnings/errors):\n";
    for (const auto& issue : detectedIssues_) {
        report += " - " + issue + "\n";
    }
    return report;
}
