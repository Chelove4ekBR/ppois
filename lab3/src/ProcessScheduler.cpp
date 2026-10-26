#include "ProcessScheduler.hpp"

#include <utility>
#include "Exceptions.hpp"

ProcessScheduler::ProcessScheduler(std::string algorithm)
    : algorithm_(std::move(algorithm)), nextPid_(1) {
    if (algorithm_.empty()) {
        throw InvalidConfigurationException("Scheduler algorithm cannot be empty");
    }
}

ProcessPtr ProcessScheduler::createProcess(std::string name, ByteSize initialMemory, uint32_t priority) {
    const uint32_t pid = nextPid_++;
    auto proc = std::make_shared<Process>(pid, std::move(name), initialMemory, priority);
    processes_.push_back(proc);
    return proc;
}

void ProcessScheduler::killProcess(uint32_t pid) {
    auto proc = findProcess(pid);
    proc->terminate();
}

ProcessPtr ProcessScheduler::findProcess(uint32_t pid) const {
    for (const auto& proc : processes_) {
        if (proc->pid() == pid) {
            return proc;
        }
    }
    throw ProcessNotFoundException("Process with PID " + std::to_string(pid) + " not found");
}

double ProcessScheduler::scheduleCycle() {
    const uint32_t count = activeProcessesCount();
    if (count == 0) {
        return 0.0;
    }
    const double perProcessLoad = 100.0 / static_cast<double>(count);
    double total = 0.0;
    for (auto& proc : processes_) {
        if (!proc->isTerminated() && proc->state() == "RUNNING") {
            proc->setCpuUsage(perProcessLoad);
            total += perProcessLoad;
        }
    }
    return total / static_cast<double>(count);
}

ByteSize ProcessScheduler::totalMemoryUsed() const {
    ByteSize total(0);
    for (const auto& proc : processes_) {
        if (!proc->isTerminated()) {
            total = total.plus(proc->memoryUsage());
        }
    }
    return total;
}

uint32_t ProcessScheduler::activeProcessesCount() const {
    uint32_t count = 0;
    for (const auto& proc : processes_) {
        if (!proc->isTerminated()) {
            ++count;
        }
    }
    return count;
}

double ProcessScheduler::totalCpuLoad() const {
    double total = 0.0;
    for (const auto& proc : processes_) {
        if (!proc->isTerminated()) {
            total += proc->cpuUsagePercent();
        }
    }
    return total;
}
