#pragma once

#include <cstdint>
#include <string>
#include "ByteSize.hpp"

/**
 * @brief Представляет вычислительную задачу (ворклоад), выполняемую процессором и памятью компьютера.
 */
class ComputerTask {
public:
    /**
     * @brief Создаёт задачу.
     * @param jobId Номер задачи.
     * @param name Название задачи (например, "Video Encoding", "3D Rendering").
     * @param requiredCycles Требуемое количество тактов/инструкций.
     * @param memoryNeeded Требуемый объём памяти.
     */
    ComputerTask(uint32_t jobId, std::string name, uint64_t requiredCycles, ByteSize memoryNeeded);

    /**
     * @brief Возвращает номер задачи.
     * @return Числовой ID.
     */
    uint32_t jobId() const { return jobId_; }

    /**
     * @brief Возвращает имя задачи.
     * @return Название.
     */
    const std::string& name() const { return name_; }

    /**
     * @brief Возвращает суммарное число требуемых тактов.
     * @return Требуемые такты.
     */
    uint64_t requiredCycles() const { return requiredCycles_; }

    /**
     * @brief Возвращает число уже выполненных тактов.
     * @return Завершённые такты.
     */
    uint64_t completedCycles() const { return completedCycles_; }

    /**
     * @brief Возвращает объём необходимой памяти.
     * @return Память ByteSize.
     */
    const ByteSize& memoryNeeded() const { return memoryNeeded_; }

    /**
     * @brief Проверяет факт полного завершения задачи.
     * @return true, если задача выполнена.
     */
    bool isComplete() const { return finished_; }

    /**
     * @brief Выполняет шаг вычислений задачи на заданное число тактов.
     * @param cycles Выполненные такты.
     */
    void progressStep(uint64_t cycles);

    /**
     * @brief Рассчитывает процент готовности задачи (0..100%).
     * @return Процент выполнения.
     */
    double completionPercentage() const;

    /**
     * @brief Сбрасывает прогресс выполнения задачи.
     */
    void reset();

    /**
     * @brief Формирует текстовое описание задачи и её статуса.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    uint32_t jobId_;
    std::string name_;
    uint64_t requiredCycles_;
    uint64_t completedCycles_;
    ByteSize memoryNeeded_;
    bool finished_;
};
