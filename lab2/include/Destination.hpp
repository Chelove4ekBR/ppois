#pragma once

#include <string>
#include <vector>

#include "Attraction.hpp"
#include "City.hpp"
#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Описывает туристическое направление, его достопримечательности и наиболее подходящие месяцы для посещения.
 */
class Destination {
public:
    /**
     * @brief Создаёт объект класса Destination и инициализирует его исходные данные.
     * @param city город.
     * @param description описание.
     * @param bestMonths список рекомендованных месяцев.
     */
    Destination(City city, std::string description, std::vector<unsigned> bestMonths);
    /**
     * @brief Возвращает город, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `city_`.
     */
    const City& city() const { return city_; }
    /**
     * @brief Возвращает описание объекта.
     * @return Ссылка на сохранённый атрибут `description_`.
     */
    const std::string& description() const { return description_; }
    /**
     * @brief Возвращает список достопримечательностей.
     * @return Ссылка на сохранённый атрибут `attractions_`.
     */
    const std::vector<Attraction>& attractions() const { return attractions_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `bestMonths_`.
     */
    const std::vector<unsigned>& bestMonths() const { return bestMonths_; }
    /**
     * @brief Добавляет достопримечательность в направление.
     * @param attraction значение параметра «attraction».
     */
    void addAttraction(const Attraction& attraction);
    /**
     * @brief Проверяет, относится ли дата к рекомендованному сезону посещения.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isBestSeason(const Date& date) const;
    /**
     * @brief Рассчитывает соответствующее значение на основе данных объекта.
     * @return Рассчитанная денежная сумма.
     */
    Money totalAttractionCost() const;
    /**
     * @brief Находит достопримечательность с минимальной стоимостью билета.
     * @return Объект или значение, полученное в результате операции.
     */
    const Attraction& cheapestAttraction() const;
    /**
     * @brief Возвращает достопримечательности, которые можно посетить за указанное время.
     * @param minutes количество минут.
     * @return Объект или значение, полученное в результате операции.
     */
    std::vector<Attraction> attractionsWithin(int minutes) const;

private:
    City city_;
    std::string description_;
    std::vector<Attraction> attractions_;
    std::vector<unsigned> bestMonths_;
};
