#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Address.hpp"
#include "DateRange.hpp"
#include "Money.hpp"
#include "Room.hpp"

/**
 * @brief Представляет отель, его категорию, адрес и список номеров.
 */
class Hotel {
public:
    /**
     * @brief Создаёт объект класса Hotel и инициализирует его исходные данные.
     * @param name название объекта.
     * @param stars количество звёзд отеля.
     * @param address адрес.
     */
    Hotel(std::string name, int stars, Address address);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `stars_`.
     */
    int stars() const { return stars_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `address_`.
     */
    const Address& address() const { return address_; }
    /**
     * @brief Возвращает список номеров отеля.
     * @return Ссылка на сохранённый атрибут `rooms_`.
     */
    const std::vector<RoomPtr>& rooms() const { return rooms_; }
    /**
     * @brief Создаёт и добавляет номер в отель.
     * @param number номер объекта.
     * @param capacity вместимость транспорта.
     * @param pricePerNight цена за ночь.
     * @return Объект или значение, полученное в результате операции.
     */
    RoomPtr addRoom(int number, int capacity, const Money& pricePerNight);
    /**
     * @brief Ищет свободный номер, подходящий для периода и количества гостей.
     * @param period период дат.
     * @param guests количество гостей.
     * @return Объект или значение, полученное в результате операции.
     */
    RoomPtr findAvailableRoom(const DateRange& period, int guests) const;
    /**
     * @brief Подсчитывает количество доступных номеров на указанный период.
     * @param period период дат.
     * @return Рассчитанное числовое значение.
     */
    std::size_t countAvailable(const DateRange& period) const;
    /**
     * @brief Рассчитывает среднюю цену.
     * @return Рассчитанная денежная сумма.
     */
    Money averagePrice() const;
    /**
     * @brief Проверяет, относится ли отель к категории люкс по правилам класса.
     * @return Результат проверки условия.
     */
    bool isLuxury() const;

private:
    std::string name_;
    int stars_;
    Address address_;
    std::vector<RoomPtr> rooms_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Hotel`.
 */
using HotelPtr = std::shared_ptr<Hotel>;
