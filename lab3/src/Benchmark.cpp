#include "Benchmark.hpp"

#include <utility>
#include "Exceptions.hpp"

namespace {
constexpr uint64_t kCpuBenchCycles = 50000000ULL;
constexpr uint32_t kGpuBenchPolygons = 2000000U;
}

Benchmark::Benchmark(std::string benchmarkName)
    : benchmarkName_(std::move(benchmarkName)), cpuScore_(0),
      gpuScore_(0), storageScore_(0), durationSeconds_(0.0) {
    if (benchmarkName_.empty()) {
        throw InvalidConfigurationException("Benchmark name cannot be empty");
    }
}

void Benchmark::runCpuBenchmark(Cpu& cpu) {
    const double execTime = cpu.executeTask(kCpuBenchCycles);
    durationSeconds_ += execTime;
    cpuScore_ = cpu.totalCores() * 1000 + static_cast<uint32_t>(cpu.cores().front().currentFrequency().toGHz() * 250.0);
}

void Benchmark::runGpuBenchmark(Gpu& gpu) {
    const double frameTimeMs = gpu.renderFrame(kGpuBenchPolygons);
    durationSeconds_ += (frameTimeMs / 1000.0);
    gpuScore_ = (gpu.cudaCores() / 10) + static_cast<uint32_t>(gpu.currentClock().toMHz());
}

void Benchmark::runStorageBenchmark(StorageDrive& drive) {
    drive.performSelfTest();
    storageScore_ = drive.readSpeedMBs() + drive.writeSpeedMBs();
    durationSeconds_ += 1.0;
}

uint32_t Benchmark::totalCompositeScore() const {
    return (cpuScore_ * 4 / 10) + (gpuScore_ * 4 / 10) + (storageScore_ * 2 / 10);
}

bool Benchmark::isPassing() const {
    return cpuScore_ > 0 && gpuScore_ > 0 && storageScore_ > 0;
}

std::string Benchmark::summary() const {
    return benchmarkName_ + " results: CPU=" + std::to_string(cpuScore_) +
           ", GPU=" + std::to_string(gpuScore_) + ", Storage=" +
           std::to_string(storageScore_) + " -> Composite=" +
           std::to_string(totalCompositeScore()) + " (took " +
           std::to_string(durationSeconds_) + "s)";
}
