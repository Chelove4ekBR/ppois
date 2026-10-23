#pragma once

#include <string>
#include <vector>

#include "City.hpp"
#include "ItineraryDay.hpp"
#include "Money.hpp"

/**
 * @brief Представляет маршрут путешествия с начальным городом и планом по дням.
 */
class Itinerary {
public:
    /**
     * @brief Создаёт объект класса Itinerary и инициализирует его исходные данные.
     * @param title название или заголовок.
     * @param startCity значение параметра «start city».
     */
    Itinerary(std::string title, City startCity);
    /**
     * @brief Возвращает заголовок или название объекта.
     * @return Ссылка на сохранённый атрибут `title_`.
     */
    const std::string& title() const { return title_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `startCity_`.
     */
    const City& startCity() const { return startCity_; }
    /**
     * @brief Возвращает список дней маршрута.
     * @return Ссылка на сохранённый атрибут `days_`.
     */
    const std::vector<ItineraryDay>& days() const { return days_; }
    /**
     * @brief Добавляет день в маршрут.
     * @param day день месяца.
     */
    void addDay(const ItineraryDay& day);
    /**
     * @brief Возвращает общее количество дней.
     * @return Рассчитанное числовое значение.
     */
    int totalDays() const;
    /**
     * @brief Рассчитывает общую стоимость.
     * @return Рассчитанная денежная сумма.
     */
    Money totalCost() const;
    /**
     * @brief Возвращает день маршрута с наибольшей суммарной нагрузкой.
     * @return Объект или значение, полученное в результате операции.
     */
    const ItineraryDay& busiestDay() const;

private:
    std::string title_;
    City startCity_;
    std::vector<ItineraryDay> days_;
};
