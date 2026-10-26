#include "SoundCard.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kHiResMinRateKhz = 96;
constexpr uint32_t kHiResMinBits = 24;
constexpr uint32_t kSurroundMinChannels = 6;
}

SoundCard::SoundCard(std::string model, uint32_t maxSampleRateKhz,
                     uint32_t bitDepth, uint32_t channels, std::string dacModel)
    : model_(std::move(model)), maxSampleRateKhz_(maxSampleRateKhz),
      bitDepth_(bitDepth), channels_(channels), dacModel_(std::move(dacModel)) {
    if (channels_ == 0 || maxSampleRateKhz_ == 0 || bitDepth_ == 0) {
        throw InvalidConfigurationException("Sound card parameters must be positive");
    }
}

bool SoundCard::supportsHighResAudio() const {
    return maxSampleRateKhz_ >= kHiResMinRateKhz && bitDepth_ >= kHiResMinBits;
}

bool SoundCard::supportsSurround() const {
    return channels_ >= kSurroundMinChannels;
}

double SoundCard::calculateAudioBitrateKbps() const {
    return static_cast<double>(maxSampleRateKhz_ * bitDepth_ * channels_);
}

double SoundCard::processAudioBuffer(uint32_t samples) {
    if (maxSampleRateKhz_ == 0) {
        throw InvalidConfigurationException("Sample rate is zero");
    }
    const double samplesPerSec = static_cast<double>(maxSampleRateKhz_) * 1000.0;
    return static_cast<double>(samples) / samplesPerSec;
}

void SoundCard::setSampleRate(uint32_t rateKhz) {
    if (rateKhz == 0 || rateKhz > maxSampleRateKhz_) {
        throw InvalidConfigurationException("Sample rate out of bounds: " + std::to_string(rateKhz));
    }
    maxSampleRateKhz_ = rateKhz;
}
