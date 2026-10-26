#include "ComputerTask.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kPercentBase = 100.0;
}

ComputerTask::ComputerTask(uint32_t jobId, std::string name, uint64_t requiredCycles, ByteSize memoryNeeded)
    : jobId_(jobId), name_(std::move(name)), requiredCycles_(requiredCycles),
      completedCycles_(0), memoryNeeded_(memoryNeeded), finished_(false) {
    if (name_.empty() || requiredCycles_ == 0) {
        throw InvalidConfigurationException("Task name cannot be empty and cycles must be positive");
    }
}

void ComputerTask::progressStep(uint64_t cycles) {
    if (finished_) {
        return;
    }
    completedCycles_ += cycles;
    if (completedCycles_ >= requiredCycles_) {
        completedCycles_ = requiredCycles_;
        finished_ = true;
    }
}

double ComputerTask::completionPercentage() const {
    if (requiredCycles_ == 0) {
        return kPercentBase;
    }
    const double comp = (static_cast<double>(completedCycles_) * kPercentBase) /
                        static_cast<double>(requiredCycles_);
    return (comp > kPercentBase) ? kPercentBase : comp;
}

void ComputerTask::reset() {
    completedCycles_ = 0;
    finished_ = false;
}

std::string ComputerTask::describe() const {
    return "Task #" + std::to_string(jobId_) + " [" + name_ + "] " +
           std::to_string(static_cast<int>(completionPercentage())) + "% done (" +
           (finished_ ? "COMPLETED" : "IN_PROGRESS") + ")";
}
