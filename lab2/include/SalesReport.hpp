#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Booking.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

/**
 * @brief Формирует сводные показатели продаж по бронированиям за заданный период.
 */
class SalesReport
{
public:
    /**
     * @brief Создаёт объект класса SalesReport и инициализирует его исходные данные.
     * @param title название или заголовок.
     * @param period период дат.
     */
    SalesReport(std::string title, const DateRange &period);
    /**
     * @brief Возвращает заголовок или название объекта.
     * @return Ссылка на сохранённый атрибут `title_`.
     */
    const std::string &title() const { return title_; }
    /**
     * @brief Возвращает период, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `period_`.
     */
    const DateRange &period() const { return period_; }
    /**
     * @brief Возвращает список бронирований.
     * @return Ссылка на сохранённый атрибут `bookings_`.
     */
    const std::vector<BookingPtr> &bookings() const { return bookings_; }
    /**
     * @brief Добавляет объект в соответствующую коллекцию или выполняет добавление по правилам класса.
     * @param booking бронирование.
     * @return Результат проверки условия.
     */
    bool add(const BookingPtr &booking);
    /**
     * @brief Возвращает количество бронирований в отчёте.
     * @return Рассчитанное числовое значение.
     */
    std::size_t bookingCount() const;
    /**
     * @brief Возвращает количество отменённых бронирований в отчёте.
     * @return Рассчитанное числовое значение.
     */
    std::size_t cancelledCount() const;
    /**
     * @brief Рассчитывает общую выручку за отчётный период.
     * @return Рассчитанная денежная сумма.
     */
    Money totalRevenue() const;
    /**
     * @brief Рассчитывает среднюю стоимость бронирования в отчёте.
     * @return Рассчитанная денежная сумма.
     */
    Money averageBookingValue() const;
    /**
     * @brief Возвращает название тура с наибольшим показателем в отчёте.
     * @return Текстовое представление или строковое значение.
     */
    std::string topTourName() const;

private:
    std::string title_;
    DateRange period_;
    std::vector<BookingPtr> bookings_;
};
