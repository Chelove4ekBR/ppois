#include "Webcam.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kBytesPerPixelRgb = 3.0;
constexpr double kBytesPerMegabyte = 1000.0 * 1000.0;
constexpr double kJpegCompressionRatio = 0.1;
}

Webcam::Webcam(std::string model, uint32_t resolutionWidth, uint32_t resolutionHeight, uint32_t fps)
    : model_(std::move(model)), resolutionWidth_(resolutionWidth),
      resolutionHeight_(resolutionHeight), fps_(fps), streaming_(false) {
    if (resolutionWidth_ == 0 || resolutionHeight_ == 0 || fps_ == 0) {
        throw InvalidConfigurationException("Webcam resolution and FPS must be positive");
    }
}

void Webcam::startStream() {
    streaming_ = true;
}

void Webcam::stopStream() {
    streaming_ = false;
}

double Webcam::bandwidthRequiredMBs() const {
    const double rawFrameBytes = static_cast<double>(resolutionWidth_ * resolutionHeight_) * kBytesPerPixelRgb;
    const double bytesPerSec = rawFrameBytes * static_cast<double>(fps_);
    return bytesPerSec / kBytesPerMegabyte;
}

ByteSize Webcam::capturePhoto() const {
    const double rawBytes = static_cast<double>(resolutionWidth_ * resolutionHeight_) * kBytesPerPixelRgb;
    const uint64_t compressedBytes = static_cast<uint64_t>(rawBytes * kJpegCompressionRatio);
    return ByteSize(compressedBytes);
}

std::string Webcam::describe() const {
    return model_ + " (" + std::to_string(resolutionWidth_) + "x" +
           std::to_string(resolutionHeight_) + " @" + std::to_string(fps_) +
           "fps, Streaming: " + (streaming_ ? "ON" : "OFF") + ")";
}
