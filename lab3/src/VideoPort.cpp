#include "VideoPort.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kHdmi20Bandwidth = 18.0;
constexpr double kHdmi21Bandwidth = 48.0;
constexpr double kDp14Bandwidth = 32.4;
constexpr double kDp20Bandwidth = 80.0;
constexpr double kBitsPerPixel = 24.0;
constexpr double kOverheadFactor = 1.25;
}

VideoPort::VideoPort(uint32_t portId, std::string portType, std::string version)
    : portId_(portId), portType_(std::move(portType)),
      version_(std::move(version)), connected_(false) {
    if (portType_.empty() || version_.empty()) {
        throw InvalidConfigurationException("Video port type and version cannot be empty");
    }
}

double VideoPort::maxBandwidthGbps() const {
    if (portType_ == "HDMI") {
        return (version_ == "2.1") ? kHdmi21Bandwidth : kHdmi20Bandwidth;
    }
    return (version_ == "2.0") ? kDp20Bandwidth : kDp14Bandwidth;
}

bool VideoPort::supportsResolution(uint32_t width, uint32_t height, uint32_t refreshHz) const {
    const double rawBitsPerSec = static_cast<double>(width) * static_cast<double>(height) *
                                 static_cast<double>(refreshHz) * kBitsPerPixel * kOverheadFactor;
    const double requiredGbps = rawBitsPerSec / 1000000000.0;
    return requiredGbps <= maxBandwidthGbps();
}

void VideoPort::connectCable() {
    if (connected_) {
        throw PortOccupiedException("Video port " + std::to_string(portId_) + " already connected");
    }
    connected_ = true;
}

void VideoPort::disconnectCable() {
    connected_ = false;
}

std::string VideoPort::describe() const {
    return portType_ + " " + version_ + " (Port #" + std::to_string(portId_) +
           (connected_ ? ", Connected)" : ", Available)");
}
