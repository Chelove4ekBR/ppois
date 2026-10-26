#include "SolidStateDrive.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kBytesPerTB = 1000.0 * 1000.0 * 1000.0 * 1000.0;
constexpr double kPercentBase = 100.0;
}

SolidStateDrive::SolidStateDrive(StorageDrive drive, std::string nandType, uint32_t tbwRatingTB)
    : drive_(std::move(drive)), nandType_(std::move(nandType)),
      tbwRatingTB_(tbwRatingTB), terabytesWritten_(0.0) {
    if (tbwRatingTB_ == 0) {
        throw InvalidConfigurationException("TBW rating must be positive");
    }
}

void SolidStateDrive::executeTrim() {
    // Освобождение неиспользуемых блоков
    drive_.performSelfTest();
}

double SolidStateDrive::calculateRemainingLifePercent() const {
    if (terabytesWritten_ >= static_cast<double>(tbwRatingTB_)) {
        return 0.0;
    }
    const double usedFraction = terabytesWritten_ / static_cast<double>(tbwRatingTB_);
    return (1.0 - usedFraction) * kPercentBase;
}

void SolidStateDrive::writePayload(ByteSize payload) {
    const double addedTB = static_cast<double>(payload.bytes()) / kBytesPerTB;
    terabytesWritten_ += addedTB;
    const double life = calculateRemainingLifePercent();
    if (life <= 0.0) {
        drive_.degradeHealth(100);
        throw DriveFailureException("SSD TBW endurance exhausted");
    }
}
