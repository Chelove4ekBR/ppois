#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет напряжение питания компонентов в милливольтах с проверкой допустимых диапазонов.
 */
class Voltage {
public:
    /**
     * @brief Создаёт объект напряжения.
     * @param millivolts Напряжение в милливольтах (мВ).
     */
    explicit Voltage(uint32_t millivolts = 0);

    /**
     * @brief Возвращает напряжение в милливольтах.
     * @return Значение в мВ.
     */
    uint32_t millivolts() const { return millivolts_; }

    /**
     * @brief Переводит значение напряжения в вольты.
     * @return Значение в В.
     */
    double toVolts() const;

    /**
     * @brief Проверяет, находится ли напряжение в безопасных эксплуатационных границах.
     * @param minSafe Минимально допустимое напряжение.
     * @param maxSafe Максимально допустимое напряжение.
     * @return true, если напряжение в безопасном диапазоне.
     */
    bool isSafe(const Voltage& minSafe, const Voltage& maxSafe) const;

    /**
     * @brief Повышает напряжение на заданную величину.
     * @param deltaMv Шаг повышения в мВ.
     * @return Новое значение напряжения.
     */
    Voltage stepUp(uint32_t deltaMv) const;

    /**
     * @brief Понижает напряжение на заданную величину.
     * @param deltaMv Шаг понижения в мВ.
     * @return Новое значение напряжения.
     */
    Voltage stepDown(uint32_t deltaMv) const;

    /**
     * @brief Сравнивает напряжение с другим значением.
     * @param other Сравниваемое напряжение.
     * @return true, если текущее напряжение выше.
     */
    bool isGreaterThan(const Voltage& other) const;

    /**
     * @brief Форматирует значение напряжения в строку.
     * @return Строка вида "1.350 V".
     */
    std::string toString() const;

private:
    uint32_t millivolts_;
};
