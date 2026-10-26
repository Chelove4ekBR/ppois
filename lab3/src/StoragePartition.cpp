#include "StoragePartition.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kPercentBase = 100.0;
}

StoragePartition::StoragePartition(std::string name, std::string fileSystemType,
                                   ByteSize capacity, std::string mountPoint)
    : name_(std::move(name)), fileSystemType_(std::move(fileSystemType)),
      capacity_(capacity), usedBytes_(ByteSize(0)), mountPoint_(std::move(mountPoint)) {
    if (name_.empty() || fileSystemType_.empty() || capacity_.bytes() == 0) {
        throw InvalidConfigurationException("Partition parameters cannot be empty/zero");
    }
}

ByteSize StoragePartition::availableBytes() const {
    return capacity_.minus(usedBytes_);
}

void StoragePartition::writeBytes(ByteSize size) {
    if (size.isGreaterThan(availableBytes())) {
        throw StorageFullException("Partition " + name_ + " is full: required " +
                                   size.toString() + ", available " + availableBytes().toString());
    }
    usedBytes_ = usedBytes_.plus(size);
}

void StoragePartition::freeBytes(ByteSize size) {
    if (size.isGreaterThan(usedBytes_)) {
        usedBytes_ = ByteSize(0);
    } else {
        usedBytes_ = usedBytes_.minus(size);
    }
}

void StoragePartition::format(std::string newFsType) {
    if (newFsType.empty()) {
        throw InvalidConfigurationException("New filesystem type cannot be empty");
    }
    fileSystemType_ = std::move(newFsType);
    usedBytes_ = ByteSize(0);
}

double StoragePartition::usagePercentage() const {
    if (capacity_.bytes() == 0) {
        return 0.0;
    }
    return (static_cast<double>(usedBytes_.bytes()) * kPercentBase) / static_cast<double>(capacity_.bytes());
}

void StoragePartition::mount(std::string point) {
    if (point.empty()) {
        throw InvalidConfigurationException("Mount point cannot be empty");
    }
    mountPoint_ = std::move(point);
}
