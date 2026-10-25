#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет тактовую частоту аппаратных компонентов и предоставляет методы конвертации и разгона.
 */
class ClockFrequency {
public:
    /**
     * @brief Создаёт объект частоты.
     * @param hertz Значение частоты в герцах (Гц).
     */
    explicit ClockFrequency(uint64_t hertz = 0);

    /**
     * @brief Создаёт объект из мегагерц (МГц).
     * @param mhz Значение в МГц.
     * @return Объект ClockFrequency.
     */
    static ClockFrequency fromMHz(uint64_t mhz);

    /**
     * @brief Создаёт объект из гигагерц (ГГц).
     * @param ghz Значение в ГГц.
     * @return Объект ClockFrequency.
     */
    static ClockFrequency fromGHz(double ghz);

    /**
     * @brief Возвращает частоту в герцах.
     * @return Значение в Гц.
     */
    uint64_t hertz() const { return hertz_; }

    /**
     * @brief Переводит значение в мегагерцы.
     * @return Частота в МГц.
     */
    double toMHz() const;

    /**
     * @brief Переводит значение в гигагерцы.
     * @return Частота в ГГц.
     */
    double toGHz() const;

    /**
     * @brief Выполняет разгон или понижение частоты на заданный процент.
     * @param percent Процент изменения (например, 10.0 для +10%).
     * @return Новое значение тактовой частоты.
     */
    ClockFrequency overclock(double percent) const;

    /**
     * @brief Складывает две тактовые частоты.
     * @param other Вторая частота.
     * @return Суммарная частота.
     */
    ClockFrequency plus(const ClockFrequency& other) const;

    /**
     * @brief Вычитает тактовую частоту.
     * @param other Вычитаемая частота.
     * @return Разность частот.
     */
    ClockFrequency minus(const ClockFrequency& other) const;

    /**
     * @brief Проверяет, строго ли больше текущая частота другой.
     * @param other Сравниваемая частота.
     * @return true, если текущая частота выше.
     */
    bool isGreaterThan(const ClockFrequency& other) const;

    /**
     * @brief Форматирует значение частоты в строку.
     * @return Строка вида "3.60 GHz" или "800.00 MHz".
     */
    std::string toString() const;

private:
    uint64_t hertz_;
};
