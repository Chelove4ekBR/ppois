#pragma once

#include <string>

#include "Money.hpp"

/**
 * @brief Описывает отдельное мероприятие в программе путешествия: длительность, стоимость и формат проведения.
 */
class Activity {
public:
    /**
     * @brief Создаёт объект класса Activity и инициализирует его исходные данные.
     * @param title название или заголовок.
     * @param durationMinutes длительность в минутах.
     * @param cost стоимость.
     * @param outdoor признак проведения на открытом воздухе.
     */
    Activity(std::string title, int durationMinutes, Money cost, bool outdoor);
    /**
     * @brief Возвращает заголовок или название объекта.
     * @return Ссылка на сохранённый атрибут `title_`.
     */
    const std::string& title() const { return title_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `durationMinutes_`.
     */
    int durationMinutes() const { return durationMinutes_; }
    /**
     * @brief Возвращает стоимость объекта.
     * @return Ссылка на сохранённый атрибут `cost_`.
     */
    const Money& cost() const { return cost_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `outdoor_`.
     */
    bool outdoor() const { return outdoor_; }
    /**
     * @brief Проверяет, превышает ли длительность заданное количество минут.
     * @param minutes количество минут.
     * @return Результат проверки условия.
     */
    bool isLongerThan(int minutes) const;
    /**
     * @brief Рассчитывает стоимость мероприятия для группы.
     * @param people количество человек.
     * @return Рассчитанная денежная сумма.
     */
    Money costForGroup(int people) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    std::string title_;
    int durationMinutes_;
    Money cost_;
    bool outdoor_;
};
