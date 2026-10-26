#include "RaidController.hpp"

#include <utility>
#include "Exceptions.hpp"

RaidController::RaidController(uint32_t raidLevel)
    : raidLevel_(raidLevel), status_("Optimal") {
    if (raidLevel_ != 0 && raidLevel_ != 1 && raidLevel_ != 5) {
        throw InvalidConfigurationException("Supported RAID levels are 0, 1, 5");
    }
}

void RaidController::attachDrive(StorageDrivePtr drive) {
    if (!drive) {
        throw ComponentNotFoundException("Cannot attach null drive to RAID");
    }
    drives_.push_back(std::move(drive));
}

ByteSize RaidController::usableCapacity() const {
    if (drives_.empty()) {
        return ByteSize(0);
    }
    const ByteSize minCap = drives_.front()->totalCapacity();
    if (raidLevel_ == 0) {
        return minCap.times(static_cast<uint32_t>(drives_.size()));
    }
    if (raidLevel_ == 1) {
        return minCap;
    }
    if (raidLevel_ == 5) {
        if (drives_.size() < 3) {
            throw InvalidConfigurationException("RAID 5 requires at least 3 drives");
        }
        return minCap.times(static_cast<uint32_t>(drives_.size() - 1));
    }
    return minCap;
}

void RaidController::handleDriveFailure(const std::string& serialNumber) {
    for (auto it = drives_.begin(); it != drives_.end(); ++it) {
        if ((*it)->serialNumber() == serialNumber) {
            drives_.erase(it);
            if (raidLevel_ == 0) {
                status_ = "Failed";
                throw DriveFailureException("RAID 0 total loss due to drive failure: " + serialNumber);
            }
            status_ = "Degraded";
            return;
        }
    }
    throw ComponentNotFoundException("Drive with serial " + serialNumber + " not in RAID");
}

void RaidController::rebuild(StorageDrivePtr replacementDrive) {
    if (!replacementDrive) {
        throw ComponentNotFoundException("Replacement drive is null");
    }
    if (status_ == "Failed") {
        throw DriveFailureException("Cannot rebuild failed array");
    }
    drives_.push_back(std::move(replacementDrive));
    status_ = "Optimal";
}

bool RaidController::isFaultTolerant() const {
    return raidLevel_ == 1 || raidLevel_ == 5;
}
