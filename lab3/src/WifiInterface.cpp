#include "WifiInterface.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr int32_t kMinSignalDbm = -90;
constexpr int32_t kMaxSignalDbm = -30;
constexpr double kPercentBase = 100.0;
}

WifiInterface::WifiInterface(NetworkAdapter adapter)
    : adapter_(std::move(adapter)), currentSsid_(""),
      signalStrengthDbm_(-60), connected_(false) {}

void WifiInterface::connectSsid(std::string ssid, const std::string& password) {
    if (ssid.empty()) {
        throw InvalidConfigurationException("Wi-Fi SSID cannot be empty");
    }
    if (password.length() < 8) {
        throw InvalidConfigurationException("WPA2/WPA3 password must be at least 8 characters");
    }
    currentSsid_ = std::move(ssid);
    connected_ = true;
    adapter_.connectLink();
}

void WifiInterface::disconnectWifi() {
    connected_ = false;
    currentSsid_.clear();
    adapter_.disconnectLink();
}

void WifiInterface::updateSignalStrength(int32_t dbm) {
    signalStrengthDbm_ = dbm;
}

double WifiInterface::signalQualityPercent() const {
    if (!connected_ || signalStrengthDbm_ <= kMinSignalDbm) {
        return 0.0;
    }
    if (signalStrengthDbm_ >= kMaxSignalDbm) {
        return kPercentBase;
    }
    const double span = static_cast<double>(kMaxSignalDbm - kMinSignalDbm);
    const double val = static_cast<double>(signalStrengthDbm_ - kMinSignalDbm);
    return (val / span) * kPercentBase;
}
