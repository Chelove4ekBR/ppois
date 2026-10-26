#include "MemorySegment.hpp"

#include "Exceptions.hpp"

MemorySegment::MemorySegment(uint64_t startAddress, ByteSize size,
                             bool readable, bool writable, bool executable)
    : startAddress_(startAddress), size_(size), allocated_(false),
      readable_(readable), writable_(writable), executable_(executable) {
    if (size_.bytes() == 0) {
        throw InvalidConfigurationException("MemorySegment size cannot be zero");
    }
}

uint64_t MemorySegment::endAddress() const {
    return startAddress_ + size_.bytes() - 1ULL;
}

void MemorySegment::allocate() {
    if (allocated_) {
        throw PortOccupiedException("MemorySegment already allocated");
    }
    allocated_ = true;
}

void MemorySegment::release() {
    allocated_ = false;
}

void MemorySegment::checkPermission(bool needWrite, bool needExec) const {
    if (needWrite && !writable_) {
        throw SegmentationFaultException("Write permission denied at memory segment 0x" +
                                         std::to_string(startAddress_));
    }
    if (needExec && !executable_) {
        throw SegmentationFaultException("Execute permission denied at memory segment 0x" +
                                         std::to_string(startAddress_));
    }
}

bool MemorySegment::overlaps(const MemorySegment& other) const {
    return (startAddress_ <= other.endAddress()) && (other.startAddress_ <= endAddress());
}

bool MemorySegment::containsAddress(uint64_t addr) const {
    return addr >= startAddress_ && addr <= endAddress();
}
