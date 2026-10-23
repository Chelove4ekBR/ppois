#pragma once

#include <string>

#include "Date.hpp"

/**
 * @brief Представляет диапазон дат и позволяет проверять включение даты и пересечение диапазонов.
 */
class DateRange {
public:
    /**
     * @brief Создаёт объект класса DateRange и инициализирует его исходные данные.
     * @param start значение параметра «start».
     * @param end значение параметра «end».
     */
    DateRange(Date start, Date end);
    /**
     * @brief Возвращает начальную дату диапазона.
     * @return Ссылка на сохранённый атрибут `start_`.
     */
    const Date& start() const { return start_; }
    /**
     * @brief Возвращает конечную дату диапазона.
     * @return Ссылка на сохранённый атрибут `end_`.
     */
    const Date& end() const { return end_; }
    /**
     * @brief Возвращает длительность диапазона в днях.
     * @return Рассчитанное числовое значение.
     */
    int lengthInDays() const;
    /**
     * @brief Проверяет, входит ли указанное значение в диапазон.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool contains(const Date& date) const;
    /**
     * @brief Проверяет, пересекаются ли два диапазона дат.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool overlaps(const DateRange& other) const;
    /**
     * @brief Преобразует значение или объект в строковое представление.
     * @return Текстовое представление или строковое значение.
     */
    std::string toString() const;

private:
    Date start_;
    Date end_;
};
