#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет термоинтерфейс (термопасту) между процессором и кулером с учётом деградации.
 */
class ThermalPaste {
public:
    /**
     * @brief Создаёт термопасту.
     * @param brand Производитель/марка термопасты.
     * @param thermalConductivity Коэффициент теплопроводности в Вт/(м·К).
     */
    ThermalPaste(std::string brand = "Arctic MX-4", double thermalConductivity = 8.5);

    /**
     * @brief Возвращает марку термопасты.
     * @return Название марки.
     */
    const std::string& brand() const { return brand_; }

    /**
     * @brief Возвращает номинальную теплопроводность.
     * @return Значение в Вт/(м·К).
     */
    double thermalConductivity() const { return thermalConductivity_; }

    /**
     * @brief Возвращает время эксплуатации в часах.
     * @return Количество часов наработки.
     */
    uint32_t appliedHours() const { return appliedHours_; }

    /**
     * @brief Увеличивает срок наработки термопасты.
     * @param hours Дополнительные часы работы.
     */
    void age(uint32_t hours);

    /**
     * @brief Проверяет, высохла ли термопаста и требует ли замены.
     * @return true, если термопаста деградировала.
     */
    bool isDegraded() const;

    /**
     * @brief Рассчитывает эффективную теплопроводность с учётом износа.
     * @return Реальная теплопроводность в Вт/(м·К).
     */
    double effectiveConductivity() const;

    /**
     * @brief Заменяет термопасту на свежую.
     * @param brand Новая марка.
     * @param newConductivity Новая теплопроводность.
     */
    void replacePaste(std::string brand, double newConductivity);

private:
    std::string brand_;
    double thermalConductivity_;
    uint32_t appliedHours_;
};
