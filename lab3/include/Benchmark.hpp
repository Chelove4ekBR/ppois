#pragma once

#include <cstdint>
#include <string>

#include "Cpu.hpp"
#include "Gpu.hpp"
#include "StorageDrive.hpp"

/**
 * @brief Выполняет комплексное тестирование производительности процессора, видеокарты и накопителей.
 */
class Benchmark {
public:
    /**
     * @brief Создаёт профиль тестирования.
     * @param benchmarkName Название бенчмарка (например, "PCMark Extreme", "3DMark TimeSpy").
     */
    explicit Benchmark(std::string benchmarkName = "System Performance Suite");

    /**
     * @brief Возвращает имя бенчмарка.
     * @return Название теста.
     */
    const std::string& benchmarkName() const { return benchmarkName_; }

    /**
     * @brief Возвращает баллы производительности CPU.
     * @return Оценка CPU.
     */
    uint32_t cpuScore() const { return cpuScore_; }

    /**
     * @brief Возвращает баллы производительности GPU.
     * @return Оценка GPU.
     */
    uint32_t gpuScore() const { return gpuScore_; }

    /**
     * @brief Возвращает баллы дисковой подсистемы.
     * @return Оценка Storage.
     */
    uint32_t storageScore() const { return storageScore_; }

    /**
     * @brief Возвращает общее время прогона тестов в секундах.
     * @return Время в секундах.
     */
    double durationSeconds() const { return durationSeconds_; }

    /**
     * @brief Запускает стресс-тест центрального процессора и рассчитывает баллы.
     * @param cpu Процессор для тестирования.
     */
    void runCpuBenchmark(Cpu& cpu);

    /**
     * @brief Запускает синтетический 3D-тест видеокарты и рассчитывает баллы.
     * @param gpu Графический процессор.
     */
    void runGpuBenchmark(Gpu& gpu);

    /**
     * @brief Запускает тест линейного и случайного чтения/записи накопителя.
     * @param drive Тестируемый накопитель.
     */
    void runStorageBenchmark(StorageDrive& drive);

    /**
     * @brief Рассчитывает суммарный комплексный рейтинг системы.
     * @return Итоговый балл (Composite score).
     */
    uint32_t totalCompositeScore() const;

    /**
     * @brief Проверяет успешность прохождения тестирования без аппаратных сбоев.
     * @return true, если все баллы положительные.
     */
    bool isPassing() const;

    /**
     * @brief Формирует итоговый отчёт бенчмарка.
     * @return Строка отчёта.
     */
    std::string summary() const;

private:
    std::string benchmarkName_;
    uint32_t cpuScore_;
    uint32_t gpuScore_;
    uint32_t storageScore_;
    double durationSeconds_;
};
