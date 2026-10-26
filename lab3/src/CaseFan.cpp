#include "CaseFan.hpp"

#include "Exceptions.hpp"

namespace {
constexpr uint32_t kMaxPwm = 100;
constexpr double kPercentBase = 100.0;
constexpr double kBaseNoiseDb = 12.0;
constexpr double kMaxNoiseFactor = 25.0;
}

CaseFan::CaseFan(uint32_t diameterMm, uint32_t maxRpm, double maxAirflowCfm)
    : diameterMm_(diameterMm), currentRpm_(maxRpm / 2),
      maxRpm_(maxRpm), pwmDuty_(50), maxAirflowCfm_(maxAirflowCfm) {
    if (diameterMm_ == 0 || maxRpm_ == 0 || maxAirflowCfm_ <= 0.0) {
        throw InvalidConfigurationException("Fan specs must be positive");
    }
}

void CaseFan::setPwmDuty(uint32_t duty) {
    if (duty > kMaxPwm) {
        throw InvalidConfigurationException("PWM duty cannot exceed 100 percent");
    }
    pwmDuty_ = duty;
    const double fraction = static_cast<double>(pwmDuty_) / kPercentBase;
    currentRpm_ = static_cast<uint32_t>(static_cast<double>(maxRpm_) * fraction);
}

double CaseFan::currentAirflowCfm() const {
    const double fraction = static_cast<double>(currentRpm_) / static_cast<double>(maxRpm_);
    return maxAirflowCfm_ * fraction;
}

double CaseFan::noiseLevelDb() const {
    if (currentRpm_ == 0) {
        return 0.0;
    }
    const double fraction = static_cast<double>(currentRpm_) / static_cast<double>(maxRpm_);
    return kBaseNoiseDb + (fraction * kMaxNoiseFactor);
}

void CaseFan::stop() {
    pwmDuty_ = 0;
    currentRpm_ = 0;
}

bool CaseFan::isSpinning() const {
    return currentRpm_ > 0;
}
