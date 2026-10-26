#include "Process.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kMaxCpu = 100.0;
}

Process::Process(uint32_t pid, std::string name, ByteSize initialMemory, uint32_t priority)
    : pid_(pid), name_(std::move(name)), memoryUsage_(initialMemory),
      cpuUsagePercent_(0.0), state_("RUNNING"), priority_(priority) {
    if (name_.empty() || priority_ == 0) {
        throw InvalidConfigurationException("Process name cannot be empty and priority must be positive");
    }
}

void Process::allocateMemory(ByteSize bytes) {
    if (isTerminated()) {
        throw ProcessNotFoundException("Cannot allocate memory for terminated process PID " + std::to_string(pid_));
    }
    memoryUsage_ = memoryUsage_.plus(bytes);
}

void Process::freeMemory(ByteSize bytes) {
    if (bytes.isGreaterThan(memoryUsage_)) {
        memoryUsage_ = ByteSize(0);
    } else {
        memoryUsage_ = memoryUsage_.minus(bytes);
    }
}

void Process::suspend() {
    if (isTerminated()) {
        throw ProcessNotFoundException("Cannot suspend terminated process PID " + std::to_string(pid_));
    }
    state_ = "SLEEPING";
    cpuUsagePercent_ = 0.0;
}

void Process::resume() {
    if (isTerminated()) {
        throw ProcessNotFoundException("Cannot resume terminated process PID " + std::to_string(pid_));
    }
    state_ = "RUNNING";
}

void Process::terminate() {
    state_ = "TERMINATED";
    cpuUsagePercent_ = 0.0;
    memoryUsage_ = ByteSize(0);
}

bool Process::isTerminated() const {
    return state_ == "TERMINATED";
}

void Process::setCpuUsage(double percent) {
    if (isTerminated() || state_ == "SLEEPING") {
        cpuUsagePercent_ = 0.0;
        return;
    }
    cpuUsagePercent_ = (percent > kMaxCpu) ? kMaxCpu : percent;
}
