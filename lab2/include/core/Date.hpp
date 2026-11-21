#pragma once

#include <string>

/**
 * @brief Представляет календарную дату и предоставляет операции сравнения и арифметики дат.
 */
class Date {
public:
    /**
     * @brief Создаёт объект класса Date и инициализирует его исходные данные.
     * @param year год.
     * @param month месяц.
     * @param day день месяца.
     */
    Date(int year, unsigned month, unsigned day);
    /**
     * @brief Возвращает год даты.
     * @return Значение сохранённого атрибута `year_`.
     */
    int year() const { return year_; }
    /**
     * @brief Возвращает месяц даты.
     * @return Значение сохранённого атрибута `month_`.
     */
    unsigned month() const { return month_; }
    /**
     * @brief Возвращает день месяца.
     * @return Значение сохранённого атрибута `day_`.
     */
    unsigned day() const { return day_; }
    /**
     * @brief Создаёт дату по числовому представлению дня.
     * @param dayNumber порядковый номер дня.
     * @return Объект или значение, полученное в результате операции.
     */
    static Date fromDayNumber(long long dayNumber);
    /**
     * @brief Возвращает числовое представление даты в днях.
     * @return Рассчитанное числовое значение.
     */
    long long dayNumber() const;
    /**
     * @brief Возвращает дату, сдвинутую на указанное количество дней.
     * @param days количество дней.
     * @return Объект или значение, полученное в результате операции.
     */
    Date addDays(int days) const;
    /**
     * @brief Рассчитывает количество дней до указанной даты.
     * @param other объект для сравнения.
     * @return Рассчитанное числовое значение.
     */
    int daysUntil(const Date& other) const;
    /**
     * @brief Проверяет, предшествует ли эта дата другой дате.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isBefore(const Date& other) const;
    /**
     * @brief Проверяет, совпадают ли две даты.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isSameDay(const Date& other) const;
    /**
     * @brief Преобразует значение или объект в строковое представление.
     * @return Текстовое представление или строковое значение.
     */
    std::string toString() const;

private:
    int year_;
    unsigned month_;
    unsigned day_;
};
