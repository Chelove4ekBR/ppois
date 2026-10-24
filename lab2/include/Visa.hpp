#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"

/**
 * @brief Представляет визу, страну назначения, тип и период действия.
 */
class Visa
{
public:
    /**
     * @brief Создаёт объект класса Visa и инициализирует его исходные данные.
     * @param country название страны.
     * @param type значение параметра «type».
     * @param validity период действия.
     */
    Visa(std::string country, std::string type, const DateRange &validity);
    /**
     * @brief Возвращает страну, связанную с объектом.
     * @return Ссылка на сохранённый атрибут `country_`.
     */
    const std::string &country() const { return country_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `type_`.
     */
    const std::string &type() const { return type_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `validity_`.
     */
    const DateRange &validity() const { return validity_; }
    /**
     * @brief Проверяет, действует ли объект на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isActiveOn(const Date &date) const;
    /**
     * @brief Проверяет, покрывает ли виза страну и весь период поездки.
     * @param countryCode код страны.
     * @param trip период поездки.
     * @return Результат проверки условия.
     */
    bool covers(const std::string &countryCode, const DateRange &trip) const;

private:
    std::string country_;
    std::string type_;
    DateRange validity_;
};
