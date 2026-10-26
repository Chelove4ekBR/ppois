#include "Display.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kMinDiagonal = 10.0;
constexpr double kMaxDiagonal = 100.0;
constexpr uint32_t kMinRefreshRate = 30;
constexpr uint32_t kMaxRefreshRate = 500;
}

Display::Display(std::string model, double diagonalInches, uint32_t resolutionWidth,
                 uint32_t resolutionHeight, uint32_t refreshRateHz, bool hdrEnabled)
    : model_(std::move(model)), diagonalInches_(diagonalInches),
      resolutionWidth_(resolutionWidth), resolutionHeight_(resolutionHeight),
      refreshRateHz_(refreshRateHz), hdrEnabled_(hdrEnabled), powerOn_(false) {
    if (diagonalInches_ < kMinDiagonal || diagonalInches_ > kMaxDiagonal ||
        resolutionWidth_ == 0 || resolutionHeight_ == 0) {
        throw InvalidConfigurationException("Invalid display parameters");
    }
}

void Display::turnOn() {
    powerOn_ = true;
}

void Display::turnOff() {
    powerOn_ = false;
}

void Display::setResolution(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) {
        throw InvalidConfigurationException("Display resolution dimensions must be positive");
    }
    resolutionWidth_ = width;
    resolutionHeight_ = height;
}

void Display::setRefreshRate(uint32_t hz) {
    if (hz < kMinRefreshRate || hz > kMaxRefreshRate) {
        throw InvalidConfigurationException("Refresh rate out of supported range: " + std::to_string(hz));
    }
    refreshRateHz_ = hz;
}

void Display::toggleHdr() {
    hdrEnabled_ = !hdrEnabled_;
}

void Display::displayFrame(uint64_t frameNumber) {
    if (!powerOn_) {
        throw DeviceNotConnectedException("Cannot display frame " + std::to_string(frameNumber) +
                                          ": monitor is powered off");
    }
}

double Display::aspectRatio() const {
    return static_cast<double>(resolutionWidth_) / static_cast<double>(resolutionHeight_);
}
