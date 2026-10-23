#pragma once

#include <memory>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Tour.hpp"

/**
 * @brief Представляет конкретное отправление тура, период проведения и учёт занятых мест.
 */
class TourDeparture {
public:
    /**
     * @brief Создаёт объект класса TourDeparture и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param tour тур.
     * @param period период дат.
     */
    TourDeparture(int id, TourPtr tour, DateRange period);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `tour_`.
     */
    const TourPtr& tour() const { return tour_; }
    /**
     * @brief Возвращает период, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `period_`.
     */
    const DateRange& period() const { return period_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `bookedSeats_`.
     */
    int bookedSeats() const { return bookedSeats_; }
    /**
     * @brief Возвращает количество свободных мест.
     * @return Рассчитанное числовое значение.
     */
    int freeSeats() const;
    /**
     * @brief Резервирует указанное количество мест.
     * @param seats количество мест.
     */
    void reserveSeats(int seats);
    /**
     * @brief Освобождает ранее зарезервированные места.
     * @param seats количество мест.
     */
    void releaseSeats(int seats);
    /**
     * @brief Проверяет, распроданы ли все места.
     * @return Результат проверки условия.
     */
    bool isSoldOut() const;
    /**
     * @brief Проверяет, начинается ли отправление в пределах заданного числа дней.
     * @param today дата, на которую выполняется проверка.
     * @param days количество дней.
     * @return Результат проверки условия.
     */
    bool startsWithin(const Date& today, int days) const;
    /**
     * @brief Проверяет, завершилось ли отправление к указанной дате.
     * @param today дата, на которую выполняется проверка.
     * @return Результат проверки условия.
     */
    bool isFinished(const Date& today) const;

private:
    int id_;
    TourPtr tour_;
    DateRange period_;
    int bookedSeats_;
};

/**
 * @brief Удобный алиас умного указателя на тип `TourDeparture`.
 */
using DeparturePtr = std::shared_ptr<TourDeparture>;
