#include "SystemLogEntry.hpp"

#include <utility>
#include "Exceptions.hpp"

SystemLogEntry::SystemLogEntry(uint64_t timestampSec, std::string level,
                               std::string subsystem, std::string message)
    : timestampSec_(timestampSec), level_(std::move(level)),
      subsystem_(std::move(subsystem)), message_(std::move(message)) {
    if (level_.empty() || subsystem_.empty()) {
        throw InvalidConfigurationException("Log level and subsystem cannot be empty");
    }
}

std::string SystemLogEntry::format() const {
    return "[" + std::to_string(timestampSec_) + "] [" + level_ + "] [" +
           subsystem_ + "] " + message_;
}

bool SystemLogEntry::isError() const {
    return level_ == "ERROR";
}

bool SystemLogEntry::matchesSubsystem(const std::string& sub) const {
    return subsystem_ == sub;
}
