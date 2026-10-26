#include "VirtualMemoryManager.hpp"

#include "Exceptions.hpp"

namespace {
constexpr uint64_t kBaseVirtualAddress = 0x00400000ULL;
}

VirtualMemoryManager::VirtualMemoryManager(ByteSize pageSize, ByteSize swapCapacity)
    : pageSize_(pageSize), swapCapacity_(swapCapacity),
      usedSwap_(ByteSize(0)), nextAddress_(kBaseVirtualAddress) {
    if (pageSize_.bytes() == 0) {
        throw InvalidConfigurationException("Page size cannot be zero");
    }
}

uint64_t VirtualMemoryManager::allocateSegment(ByteSize size, bool writable, bool executable) {
    if (size.bytes() == 0) {
        throw InvalidConfigurationException("Cannot allocate zero-byte segment");
    }
    const uint64_t allocatedAddr = nextAddress_;
    MemorySegment segment(allocatedAddr, size, true, writable, executable);
    segment.allocate();
    segments_.push_back(segment);
    nextAddress_ += size.bytes();
    return allocatedAddr;
}

void VirtualMemoryManager::freeSegment(uint64_t startAddress) {
    for (auto it = segments_.begin(); it != segments_.end(); ++it) {
        if (it->startAddress() == startAddress) {
            it->release();
            segments_.erase(it);
            return;
        }
    }
    throw InvalidAddressException("Memory segment not found at address 0x" + std::to_string(startAddress));
}

void VirtualMemoryManager::handlePageFault(ByteSize neededSize) {
    if (neededSize.isGreaterThan(availableSwap())) {
        throw InsufficientMemoryException("Virtual memory exhausted: cannot swap in " + neededSize.toString());
    }
    usedSwap_ = usedSwap_.plus(neededSize);
}

ByteSize VirtualMemoryManager::totalAllocatedBytes() const {
    ByteSize total(0);
    for (const auto& seg : segments_) {
        if (seg.isAllocated()) {
            total = total.plus(seg.size());
        }
    }
    return total;
}

const MemorySegment& VirtualMemoryManager::findSegment(uint64_t addr) const {
    for (const auto& seg : segments_) {
        if (seg.containsAddress(addr)) {
            return seg;
        }
    }
    throw InvalidAddressException("Address 0x" + std::to_string(addr) + " does not belong to any segment");
}

ByteSize VirtualMemoryManager::availableSwap() const {
    if (usedSwap_.isGreaterThan(swapCapacity_)) {
        return ByteSize(0);
    }
    return swapCapacity_.minus(usedSwap_);
}
