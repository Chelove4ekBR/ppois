#include "AudioOutput.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kMaxVolume = 100;
}

AudioOutput::AudioOutput(std::string deviceName, std::string deviceType, uint32_t volumePercent)
    : deviceName_(std::move(deviceName)), deviceType_(std::move(deviceType)),
      volumePercent_(volumePercent), muted_(false) {
    if (volumePercent_ > kMaxVolume) {
        throw InvalidConfigurationException("Volume cannot exceed 100 percent");
    }
}

void AudioOutput::setVolume(uint32_t percent) {
    if (percent > kMaxVolume) {
        throw InvalidConfigurationException("Volume cannot exceed 100 percent: " + std::to_string(percent));
    }
    volumePercent_ = percent;
}

void AudioOutput::mute() {
    muted_ = true;
}

void AudioOutput::unmute() {
    muted_ = false;
}

void AudioOutput::toggleMute() {
    muted_ = !muted_;
}

uint32_t AudioOutput::effectiveVolume() const {
    return muted_ ? 0 : volumePercent_;
}

void AudioOutput::playSound(uint32_t durationMs) {
    if (effectiveVolume() == 0 && !muted_) {
        // Воспроизведение беззвучно
        return;
    }
    if (durationMs == 0) {
        throw InvalidConfigurationException("Playback duration must be positive");
    }
}
