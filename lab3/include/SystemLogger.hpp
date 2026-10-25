#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "SystemLogEntry.hpp"

/**
 * @brief Представляет централизованный регистратор системных событий и диагностических логов (Logger).
 */
class SystemLogger {
public:
    /**
     * @brief Создаёт логгер.
     * @param maxEntries Предельный размер кольцевого буфера логов.
     */
    explicit SystemLogger(std::size_t maxEntries = 1000);

    /**
     * @brief Возвращает все накопленные записи журнала.
     * @return Вектор SystemLogEntry.
     */
    const std::vector<SystemLogEntry>& entries() const { return entries_; }

    /**
     * @brief Возвращает предел записей.
     * @return Ёмкость буфера.
     */
    std::size_t maxEntries() const { return maxEntries_; }

    /**
     * @brief Добавляет новую запись в журнал.
     * @param level Уровень важности ("INFO", "WARN", "ERROR").
     * @param subsystem Источник события.
     * @param message Текст сообщения.
     * @param timestamp Метка времени.
     */
    void log(const std::string& level, const std::string& subsystem,
             const std::string& message, uint64_t timestamp = 0);

    /**
     * @brief Рассчитывает количество зафиксированных критических ошибок (ERROR).
     * @return Число ошибок.
     */
    std::size_t errorCount() const;

    /**
     * @brief Фильтрует записи журнала по заданному уровню важности.
     * @param level Уровень важности.
     * @return Список отфильтрованных записей.
     */
    std::vector<SystemLogEntry> getEntriesByLevel(const std::string& level) const;

    /**
     * @brief Фильтрует записи журнала по имени подсистемы.
     * @param subsystem Название подсистемы.
     * @return Список записей.
     */
    std::vector<SystemLogEntry> getEntriesBySubsystem(const std::string& subsystem) const;

    /**
     * @brief Очищает все записи в журнале.
     */
    void clear();

    /**
     * @brief Возвращает общее количество сохранённых записей.
     * @return Число записей.
     */
    std::size_t totalEntries() const;

private:
    std::vector<SystemLogEntry> entries_;
    std::size_t maxEntries_;
};
