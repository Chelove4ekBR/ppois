#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "Process.hpp"

/**
 * @brief Представляет планировщик процессов операционной системы (Scheduler).
 */
class ProcessScheduler {
public:
    /**
     * @brief Создаёт планировщик процессов.
     * @param algorithm Алгоритм планирования ("RoundRobin", "FIFO").
     */
    explicit ProcessScheduler(std::string algorithm = "RoundRobin");

    /**
     * @brief Возвращает алгоритм планирования.
     * @return Название алгоритма.
     */
    const std::string& algorithm() const { return algorithm_; }

    /**
     * @brief Возвращает список управляемых процессов.
     * @return Вектор умных указателей на Process.
     */
    const std::vector<ProcessPtr>& processes() const { return processes_; }

    /**
     * @brief Создаёт и регистрирует новый процесс.
     * @param name Название процесса.
     * @param initialMemory Выделяемый объём памяти.
     * @param priority Приоритет (1..20).
     * @return Умный указатель на созданный процесс.
     */
    ProcessPtr createProcess(std::string name, ByteSize initialMemory, uint32_t priority = 10);

    /**
     * @brief Завершает процесс по PID.
     * @param pid Идентификатор процесса.
     */
    void killProcess(uint32_t pid);

    /**
     * @brief Находит процесс по его PID.
     * @param pid Идентификатор процесса.
     * @return Умный указатель на Process.
     */
    ProcessPtr findProcess(uint32_t pid) const;

    /**
     * @brief Выполняет квант планирования и распределения процессорного времени между процессами.
     * @return Средняя нагрузка процессов за такт.
     */
    double scheduleCycle();

    /**
     * @brief Рассчитывает суммарную память, занятую всеми активными процессами.
     * @return Суммарный объём ByteSize.
     */
    ByteSize totalMemoryUsed() const;

    /**
     * @brief Возвращает число активных (не завершённых) процессов.
     * @return Число процессов.
     */
    uint32_t activeProcessesCount() const;

    /**
     * @brief Рассчитывает суммарную нагрузку CPU всех процессов.
     * @return Процент загрузки.
     */
    double totalCpuLoad() const;

private:
    std::string algorithm_;
    std::vector<ProcessPtr> processes_;
    uint32_t nextPid_;
};
