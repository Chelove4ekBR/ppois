#include "Gpu.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr double kMilliToSeconds = 1000.0;
constexpr double kHeatPerMillionPolygons = 0.5;
constexpr double kHeatPerCompute = 0.4;
}

Gpu::Gpu(std::string model, uint32_t cudaCores, ClockFrequency baseClock, uint32_t tdpWatt)
    : model_(std::move(model)), cudaCores_(cudaCores),
      baseClock_(baseClock), currentClock_(baseClock),
      tdpWatt_(tdpWatt), thermalZone_(38.0, 83.0, 95.0) {
    if (cudaCores_ == 0 || baseClock_.hertz() == 0) {
        throw InvalidConfigurationException("GPU must have cores and non-zero frequency");
    }
}

double Gpu::renderFrame(uint32_t polygonCount) {
    if (isOverheating()) {
        currentClock_ = currentClock_.overclock(-20.0);
    }
    const double cyclesNeeded = static_cast<double>(polygonCount) / static_cast<double>(cudaCores_);
    const double frameTimeMs = (cyclesNeeded / currentClock_.toMHz()) * 10.0;
    const double heating = (static_cast<double>(polygonCount) / 1000000.0) * kHeatPerMillionPolygons;
    thermalZone_.updateTemperature(heating);
    return frameTimeMs;
}

double Gpu::computeShader(uint32_t workUnits) {
    const double ops = static_cast<double>(workUnits) * 1000.0;
    const double seconds = ops / (static_cast<double>(cudaCores_) * currentClock_.toMHz() * 1000.0);
    thermalZone_.updateTemperature(kHeatPerCompute);
    return seconds * kMilliToSeconds;
}

void Gpu::overclock(double percent) {
    currentClock_ = baseClock_.overclock(percent);
}

void Gpu::coolDown(double deltaC) {
    thermalZone_.cool(deltaC);
}

bool Gpu::isOverheating() const {
    return thermalZone_.isThrottling() || thermalZone_.isCritical();
}
