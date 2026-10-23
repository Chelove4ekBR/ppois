#pragma once

#include <string>

#include "Country.hpp"

/**
 * @brief Хранит сведения о городе, стране и смещении местного времени относительно UTC.
 */
class City {
public:
    /**
     * @brief Создаёт объект класса City и инициализирует его исходные данные.
     * @param name название объекта.
     * @param country название страны.
     * @param utcOffsetHours значение параметра «utc offset hours».
     */
    City(std::string name, Country country, int utcOffsetHours);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает страну, связанную с объектом.
     * @return Ссылка на сохранённый атрибут `country_`.
     */
    const Country& country() const { return country_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `utcOffsetHours_`.
     */
    int utcOffsetHours() const { return utcOffsetHours_; }
    /**
     * @brief Формирует краткую подпись объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string label() const;
    /**
     * @brief Проверяет, расположен ли город в указанной стране.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isInCountry(const Country& other) const;
    /**
     * @brief Рассчитывает разницу местного времени между городами в часах.
     * @param other объект для сравнения.
     * @return Рассчитанное числовое значение.
     */
    int timeDifferenceHours(const City& other) const;

private:
    std::string name_;
    Country country_;
    int utcOffsetHours_;
};
