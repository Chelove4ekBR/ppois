#include "StorageDrive.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kMaxHealth = 100;
constexpr uint32_t kMinUsableHealth = 10;
}

StorageDrive::StorageDrive(std::string model, std::string serialNumber, std::string interfaceType,
                           ByteSize totalCapacity, uint32_t readSpeedMBs, uint32_t writeSpeedMBs)
    : model_(std::move(model)), serialNumber_(std::move(serialNumber)),
      interfaceType_(std::move(interfaceType)), totalCapacity_(totalCapacity),
      readSpeedMBs_(readSpeedMBs), writeSpeedMBs_(writeSpeedMBs), healthPercent_(kMaxHealth) {
    if (serialNumber_.empty() || totalCapacity_.bytes() == 0) {
        throw InvalidConfigurationException("Drive must have valid serial and capacity");
    }
}

void StorageDrive::addPartition(const StoragePartition& partition) {
    ByteSize allocated(0);
    for (const auto& p : partitions_) {
        allocated = allocated.plus(p.capacity());
    }
    const ByteSize nextAllocated = allocated.plus(partition.capacity());
    if (nextAllocated.isGreaterThan(totalCapacity_)) {
        throw StorageFullException("Cannot add partition: exceeds physical drive capacity");
    }
    partitions_.push_back(partition);
}

StoragePartition& StorageDrive::findPartition(const std::string& name) {
    for (auto& p : partitions_) {
        if (p.name() == name) {
            return p;
        }
    }
    throw ComponentNotFoundException("Partition not found: " + name);
}

ByteSize StorageDrive::totalUsedBytes() const {
    ByteSize total(0);
    for (const auto& p : partitions_) {
        total = total.plus(p.usedBytes());
    }
    return total;
}

ByteSize StorageDrive::unpartitionedCapacity() const {
    ByteSize allocated(0);
    for (const auto& p : partitions_) {
        allocated = allocated.plus(p.capacity());
    }
    return totalCapacity_.minus(allocated);
}

void StorageDrive::degradeHealth(uint32_t amount) {
    if (amount >= healthPercent_) {
        healthPercent_ = 0;
        throw DriveFailureException("Storage drive " + serialNumber_ + " failed permanently");
    }
    healthPercent_ -= amount;
}

bool StorageDrive::performSelfTest() {
    if (healthPercent_ < kMinUsableHealth) {
        throw DriveFailureException("S.M.A.R.T. self-test failed: critical health " +
                                    std::to_string(healthPercent_) + "%");
    }
    return true;
}
