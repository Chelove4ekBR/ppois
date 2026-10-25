#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет отдельную запись в системном журнале событий (лог).
 */
class SystemLogEntry {
public:
    /**
     * @brief Создаёт запись системного журнала.
     * @param timestampSec Метка времени в секундах.
     * @param level Уровень важности ("INFO", "WARN", "ERROR").
     * @param subsystem Имя подсистемы-источника ("KERNEL", "GPU", "STORAGE", "NETWORK").
     * @param message Текст сообщения.
     */
    SystemLogEntry(uint64_t timestampSec, std::string level, std::string subsystem, std::string message);

    /**
     * @brief Возвращает временную метку.
     * @return Время в секундах.
     */
    uint64_t timestampSec() const { return timestampSec_; }

    /**
     * @brief Возвращает уровень важности.
     * @return Строка ("INFO", "WARN", "ERROR").
     */
    const std::string& level() const { return level_; }

    /**
     * @brief Возвращает подсистему-источник.
     * @return Имя подсистемы.
     */
    const std::string& subsystem() const { return subsystem_; }

    /**
     * @brief Возвращает текст сообщения.
     * @return Сообщение.
     */
    const std::string& message() const { return message_; }

    /**
     * @brief Форматирует запись в строку вида "[TIME] [LEVEL] [SUBSYSTEM] Message".
     * @return Отформатированная строка.
     */
    std::string format() const;

    /**
     * @brief Проверяет, является ли запись сообщением об ошибке (ERROR).
     * @return true, если уровень равен ERROR.
     */
    bool isError() const;

    /**
     * @brief Проверяет принадлежность записи указанной подсистеме.
     * @param sub Подсистема для сравнения.
     * @return true, если подсистема совпадает.
     */
    bool matchesSubsystem(const std::string& sub) const;

private:
    uint64_t timestampSec_;
    std::string level_;
    std::string subsystem_;
    std::string message_;
};
