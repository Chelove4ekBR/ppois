#include "SystemLogger.hpp"

#include "Exceptions.hpp"

SystemLogger::SystemLogger(std::size_t maxEntries) : maxEntries_(maxEntries) {
    if (maxEntries_ == 0) {
        throw InvalidConfigurationException("Logger maxEntries must be greater than zero");
    }
}

void SystemLogger::log(const std::string& level, const std::string& subsystem,
                       const std::string& message, uint64_t timestamp) {
    if (entries_.size() >= maxEntries_) {
        entries_.erase(entries_.begin());
    }
    entries_.emplace_back(timestamp, level, subsystem, message);
}

std::size_t SystemLogger::errorCount() const {
    std::size_t count = 0;
    for (const auto& entry : entries_) {
        if (entry.isError()) {
            ++count;
        }
    }
    return count;
}

std::vector<SystemLogEntry> SystemLogger::getEntriesByLevel(const std::string& level) const {
    std::vector<SystemLogEntry> result;
    for (const auto& entry : entries_) {
        if (entry.level() == level) {
            result.push_back(entry);
        }
    }
    return result;
}

std::vector<SystemLogEntry> SystemLogger::getEntriesBySubsystem(const std::string& subsystem) const {
    std::vector<SystemLogEntry> result;
    for (const auto& entry : entries_) {
        if (entry.matchesSubsystem(subsystem)) {
            result.push_back(entry);
        }
    }
    return result;
}

void SystemLogger::clear() {
    entries_.clear();
}

std::size_t SystemLogger::totalEntries() const {
    return entries_.size();
}
