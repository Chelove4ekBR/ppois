#include "Mouse.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint32_t kMinDpi = 200;
constexpr uint32_t kMaxDpi = 32000;
constexpr uint32_t kHighPrecisionDpi = 3200;
constexpr uint32_t kHighPrecisionPolling = 1000;
}

Mouse::Mouse(std::string model, uint32_t currentDpi, uint32_t buttonCount, uint32_t pollingRateHz)
    : model_(std::move(model)), currentDpi_(currentDpi),
      buttonCount_(buttonCount), pollingRateHz_(pollingRateHz), clickCount_(0) {
    if (buttonCount_ < 1 || currentDpi_ < kMinDpi || currentDpi_ > kMaxDpi) {
        throw InvalidConfigurationException("Invalid mouse parameters");
    }
}

void Mouse::setDpi(uint32_t dpi) {
    if (dpi < kMinDpi || dpi > kMaxDpi) {
        throw InvalidConfigurationException("DPI out of bounds: " + std::to_string(dpi));
    }
    currentDpi_ = dpi;
}

void Mouse::click(uint32_t buttonId) {
    if (buttonId == 0 || buttonId > buttonCount_) {
        throw ComponentNotFoundException("Mouse button #" + std::to_string(buttonId) + " does not exist");
    }
    ++clickCount_;
}

double Mouse::move(int32_t deltaX, int32_t deltaY) {
    const double dx = static_cast<double>(deltaX);
    const double dy = static_cast<double>(deltaY);
    return std::sqrt(dx * dx + dy * dy);
}

bool Mouse::isHighPrecision() const {
    return currentDpi_ >= kHighPrecisionDpi && pollingRateHz_ >= kHighPrecisionPolling;
}

std::string Mouse::describe() const {
    return model_ + " (DPI: " + std::to_string(currentDpi_) + ", Buttons: " +
           std::to_string(buttonCount_) + ", " + std::to_string(pollingRateHz_) + " Hz)";
}
