#include "Keyboard.hpp"

#include <utility>
#include "Exceptions.hpp"

Keyboard::Keyboard(std::string model, std::string layout, bool isMechanical, bool backlightActive)
    : model_(std::move(model)), layout_(std::move(layout)),
      isMechanical_(isMechanical), backlightActive_(backlightActive), pressedKeysCount_(0) {
    if (model_.empty() || layout_.empty()) {
        throw InvalidConfigurationException("Keyboard model and layout cannot be empty");
    }
}

void Keyboard::pressKey(const std::string& key) {
    if (key.empty()) {
        throw InvalidConfigurationException("Cannot press empty key");
    }
    ++pressedKeysCount_;
}

void Keyboard::switchLayout(std::string newLayout) {
    if (newLayout.empty()) {
        throw InvalidConfigurationException("New layout cannot be empty");
    }
    layout_ = std::move(newLayout);
}

void Keyboard::toggleBacklight() {
    backlightActive_ = !backlightActive_;
}

void Keyboard::resetKeys() {
    pressedKeysCount_ = 0;
}

std::string Keyboard::describe() const {
    return model_ + " (" + layout_ + ", " +
           (isMechanical_ ? "Mechanical" : "Membrane") +
           ", Backlight: " + (backlightActive_ ? "ON" : "OFF") + ")";
}
