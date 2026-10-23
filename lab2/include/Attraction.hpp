#pragma once

#include <string>

#include "Money.hpp"

/**
 * @brief Описывает туристическую достопримечательность, стоимость билета и примерную продолжительность посещения.
 */
class Attraction {
public:
    /**
     * @brief Создаёт объект класса Attraction и инициализирует его исходные данные.
     * @param name название объекта.
     * @param ticketPrice значение параметра «ticket price».
     * @param visitMinutes значение параметра «visit minutes».
     * @param indoor признак проведения в помещении.
     */
    Attraction(std::string name, Money ticketPrice, int visitMinutes, bool indoor);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `ticketPrice_`.
     */
    const Money& ticketPrice() const { return ticketPrice_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `visitMinutes_`.
     */
    int visitMinutes() const { return visitMinutes_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `indoor_`.
     */
    bool indoor() const { return indoor_; }
    /**
     * @brief Проверяет, является ли посещение бесплатным.
     * @return Результат проверки условия.
     */
    bool isFree() const;
    /**
     * @brief Рассчитывает стоимость с учётом заданного количества человек или дней.
     * @param visitors количество посетителей.
     * @return Рассчитанная денежная сумма.
     */
    Money costFor(int visitors) const;
    /**
     * @brief Проверяет, помещается ли действие в доступный временной интервал.
     * @param availableMinutes доступное время в минутах.
     * @return Результат проверки условия.
     */
    bool fitsInto(int availableMinutes) const;

private:
    std::string name_;
    Money ticketPrice_;
    int visitMinutes_;
    bool indoor_;
};
