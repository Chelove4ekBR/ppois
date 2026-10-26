#include "Chipset.hpp"

#include <utility>
#include "Exceptions.hpp"

Chipset::Chipset(std::string model, std::string supportedSocket,
                 uint32_t maxPcieLanes, bool overclockingSupported)
    : model_(std::move(model)), supportedSocket_(std::move(supportedSocket)),
      maxPcieLanes_(maxPcieLanes), overclockingSupported_(overclockingSupported) {
    if (model_.empty() || supportedSocket_.empty()) {
        throw InvalidConfigurationException("Chipset model and socket cannot be empty");
    }
}

bool Chipset::supportsSocket(const std::string& socket) const {
    return supportedSocket_ == socket;
}

bool Chipset::validatePcieLanes(uint32_t lanes) const {
    return lanes <= maxPcieLanes_;
}

std::string Chipset::describe() const {
    return model_ + " (" + supportedSocket_ + ", PCIe lanes: " +
           std::to_string(maxPcieLanes_) + (overclockingSupported_ ? ", OC enabled)" : ")");
}
