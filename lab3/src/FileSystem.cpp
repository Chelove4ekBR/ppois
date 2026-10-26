#include "FileSystem.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kMaxFragmentation = 15.0;
}

FileSystem::FileSystem(std::string name, ByteSize clusterSize, bool journaled)
    : name_(std::move(name)), clusterSize_(clusterSize),
      totalFiles_(0), journaled_(journaled) {
    if (name_.empty() || clusterSize_.bytes() == 0) {
        throw InvalidConfigurationException("Invalid filesystem parameters");
    }
}

uint64_t FileSystem::calculateClustersNeeded(ByteSize size) const {
    if (size.bytes() == 0) {
        return 0;
    }
    const uint64_t clusters = (size.bytes() + clusterSize_.bytes() - 1ULL) / clusterSize_.bytes();
    return clusters;
}

void FileSystem::createFile(ByteSize fileSize, StoragePartition& partition) {
    const uint64_t clusters = calculateClustersNeeded(fileSize);
    const ByteSize physicalSize(clusters * clusterSize_.bytes());
    partition.writeBytes(physicalSize);
    ++totalFiles_;
}

void FileSystem::deleteFile(ByteSize fileSize, StoragePartition& partition) {
    if (totalFiles_ == 0) {
        throw ComponentNotFoundException("No files to delete in filesystem");
    }
    const uint64_t clusters = calculateClustersNeeded(fileSize);
    const ByteSize physicalSize(clusters * clusterSize_.bytes());
    partition.freeBytes(physicalSize);
    --totalFiles_;
}

bool FileSystem::checkConsistency() const {
    return journaled_ || totalFiles_ < 100000;
}

double FileSystem::fragmentationPercent() const {
    if (totalFiles_ == 0) {
        return 0.0;
    }
    const double frag = static_cast<double>(totalFiles_ % 10);
    return (frag > kMaxFragmentation) ? kMaxFragmentation : frag;
}
