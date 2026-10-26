#include "ComputerCase.hpp"

#include <utility>
#include "Exceptions.hpp"

ComputerCase::ComputerCase(std::string model, std::string formFactorSupported,
                           uint32_t driveBaysCount, uint32_t maxGpuLengthMm)
    : model_(std::move(model)), formFactorSupported_(std::move(formFactorSupported)),
      driveBaysCount_(driveBaysCount), maxGpuLengthMm_(maxGpuLengthMm) {
    if (model_.empty() || maxGpuLengthMm_ == 0) {
        throw InvalidConfigurationException("Invalid computer case parameters");
    }
    fans_.emplace_back(140, 1500, 75.0); // Передний на вдув
    fans_.emplace_back(120, 1800, 60.0); // Задний на выдув
}

void ComputerCase::installFan(const CaseFan& fan) {
    fans_.push_back(fan);
}

double ComputerCase::totalAirflowCfm() const {
    double total = 0.0;
    for (const auto& fan : fans_) {
        total += fan.currentAirflowCfm();
    }
    return total;
}

bool ComputerCase::canFitGpu(uint32_t lengthMm) const {
    return lengthMm <= maxGpuLengthMm_;
}

void ComputerCase::setAllFansPwm(uint32_t duty) {
    for (auto& fan : fans_) {
        fan.setPwmDuty(duty);
    }
}

double ComputerCase::averageNoiseDb() const {
    if (fans_.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const auto& fan : fans_) {
        sum += fan.noiseLevelDb();
    }
    return sum / static_cast<double>(fans_.size());
}
