#pragma once

#include <memory>
#include <vector>

#include "DateRange.hpp"
#include "Money.hpp"

/**
 * @brief Представляет номер отеля, его вместимость, цену за ночь и периоды бронирования.
 */
class Room {
public:
    /**
     * @brief Создаёт объект класса Room и инициализирует его исходные данные.
     * @param number номер объекта.
     * @param capacity вместимость транспорта.
     * @param pricePerNight цена за ночь.
     */
    Room(int number, int capacity, Money pricePerNight);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `number_`.
     */
    int number() const { return number_; }
    /**
     * @brief Возвращает вместимость объекта.
     * @return Значение сохранённого атрибута `capacity_`.
     */
    int capacity() const { return capacity_; }
    /**
     * @brief Возвращает цену за одну ночь.
     * @return Ссылка на сохранённый атрибут `pricePerNight_`.
     */
    const Money& pricePerNight() const { return pricePerNight_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `bookedPeriods_`.
     */
    const std::vector<DateRange>& bookedPeriods() const { return bookedPeriods_; }
    /**
     * @brief Проверяет доступность объекта на указанный период.
     * @param period период дат.
     * @return Результат проверки условия.
     */
    bool isAvailable(const DateRange& period) const;
    /**
     * @brief Проверяет, подходит ли объект по вместимости.
     * @param guests количество гостей.
     * @return Результат проверки условия.
     */
    bool fits(int guests) const;
    /**
     * @brief Резервирует объект на указанный период.
     * @param period период дат.
     */
    void reserve(const DateRange& period);
    /**
     * @brief Проверяет возможность выполнить действие: cel reservation.
     * @param period период дат.
     */
    void cancelReservation(const DateRange& period);
    /**
     * @brief Рассчитывает стоимость с учётом заданного количества человек или дней.
     * @param period период дат.
     * @return Рассчитанная денежная сумма.
     */
    Money costFor(const DateRange& period) const;

private:
    int number_;
    int capacity_;
    Money pricePerNight_;
    std::vector<DateRange> bookedPeriods_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Room`.
 */
using RoomPtr = std::shared_ptr<Room>;
