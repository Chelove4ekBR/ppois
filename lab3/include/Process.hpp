#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "ByteSize.hpp"

/**
 * @brief Представляет выполняющийся процесс операционной системы.
 */
class Process {
public:
    /**
     * @brief Создаёт процесс.
     * @param pid Идентификатор процесса (PID).
     * @param name Имя исполняемого файла / программы.
     * @param initialMemory Начально выделенная память.
     * @param priority Приоритет диспетчеризации (1..20, где 1 - наивысший).
     */
    Process(uint32_t pid, std::string name, ByteSize initialMemory, uint32_t priority = 10);

    /**
     * @brief Возвращает PID процесса.
     * @return Числовой идентификатор.
     */
    uint32_t pid() const { return pid_; }

    /**
     * @brief Возвращает имя процесса.
     * @return Название программы.
     */
    const std::string& name() const { return name_; }

    /**
     * @brief Возвращает используемый объём памяти.
     * @return Объём ByteSize.
     */
    const ByteSize& memoryUsage() const { return memoryUsage_; }

    /**
     * @brief Возвращает текущее потребление ресурсов CPU.
     * @return Процент CPU (0..100%).
     */
    double cpuUsagePercent() const { return cpuUsagePercent_; }

    /**
     * @brief Возвращает текущее состояние процесса.
     * @return Строка ("RUNNING", "SLEEPING", "TERMINATED").
     */
    const std::string& state() const { return state_; }

    /**
     * @brief Возвращает приоритет процесса.
     * @return Значение приоритета.
     */
    uint32_t priority() const { return priority_; }

    /**
     * @brief Выделяет дополнительную память процессу.
     * @param bytes Объём запрашиваемой памяти.
     */
    void allocateMemory(ByteSize bytes);

    /**
     * @brief Освобождает часть памяти процесса.
     * @param bytes Объём освобождаемой памяти.
     */
    void freeMemory(ByteSize bytes);

    /**
     * @brief Переводит процесс в состояние сна (SLEEPING).
     */
    void suspend();

    /**
     * @brief Возобновляет выполнение процесса (RUNNING).
     */
    void resume();

    /**
     * @brief Завершает процесс (TERMINATED).
     */
    void terminate();

    /**
     * @brief Проверяет факт завершения процесса.
     * @return true, если процесс остановлен.
     */
    bool isTerminated() const;

    /**
     * @brief Устанавливает текущее потребление процессора.
     * @param percent Процент загрузки CPU.
     */
    void setCpuUsage(double percent);

private:
    uint32_t pid_;
    std::string name_;
    ByteSize memoryUsage_;
    double cpuUsagePercent_;
    std::string state_;
    uint32_t priority_;
};

using ProcessPtr = std::shared_ptr<Process>;
