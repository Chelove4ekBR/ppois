#include "HardDiskDrive.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kRotationBaseMs = 30000.0;
constexpr double kBaseHeadSeekMs = 4.0;
constexpr uint32_t kMaxBadSectorsAllowed = 500;
}

HardDiskDrive::HardDiskDrive(StorageDrive drive, uint32_t spindleSpeedRpm)
    : drive_(std::move(drive)), spindleSpeedRpm_(spindleSpeedRpm),
      badSectorsCount_(0), headsParked_(true) {
    if (spindleSpeedRpm_ == 0) {
        throw InvalidConfigurationException("Spindle speed cannot be zero");
    }
}

double HardDiskDrive::seekTimeMs() const {
    return (kRotationBaseMs / static_cast<double>(spindleSpeedRpm_)) + kBaseHeadSeekMs;
}

void HardDiskDrive::parkHeads() {
    headsParked_ = true;
}

void HardDiskDrive::unparkHeads() {
    headsParked_ = false;
}

void HardDiskDrive::remapBadSectors(uint32_t count) {
    badSectorsCount_ += count;
    drive_.degradeHealth(count / 10 + 1);
    if (badSectorsCount_ > kMaxBadSectorsAllowed) {
        throw DriveFailureException("Hard drive bad sectors limit exceeded (" +
                                    std::to_string(badSectorsCount_) + ")");
    }
}
