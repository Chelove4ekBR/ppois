#pragma once

#include <string>

#include "Address.hpp"
#include "Bus.hpp"
#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Представляет трансфер между адресами с автобусом, датой подачи и ценой на пассажира.
 */
class Transfer {
public:
    /**
     * @brief Создаёт объект класса Transfer и инициализирует его исходные данные.
     * @param from адрес или начальное значение.
     * @param to адрес или конечное значение.
     * @param bus значение параметра «bus».
     * @param pickupDate дата подачи транспорта.
     * @param pricePerPassenger цена на пассажира.
     */
    Transfer(Address from, Address to, BusPtr bus, Date pickupDate, Money pricePerPassenger);
    /**
     * @brief Возвращает начальный адрес или исходное значение операции.
     * @return Ссылка на сохранённый атрибут `from_`.
     */
    const Address& from() const { return from_; }
    /**
     * @brief Возвращает конечный адрес или целевое значение операции.
     * @return Ссылка на сохранённый атрибут `to_`.
     */
    const Address& to() const { return to_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `bus_`.
     */
    const BusPtr& bus() const { return bus_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `pickupDate_`.
     */
    const Date& pickupDate() const { return pickupDate_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `pricePerPassenger_`.
     */
    const Money& pricePerPassenger() const { return pricePerPassenger_; }
    /**
     * @brief Проверяет, проходит ли трансфер в пределах одного города.
     * @return Результат проверки условия.
     */
    bool isIntraCity() const;
    /**
     * @brief Рассчитывает стоимость для указанного количества участников.
     * @param passengers количество пассажиров.
     * @return Рассчитанная денежная сумма.
     */
    Money priceFor(int passengers) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    Address from_;
    Address to_;
    BusPtr bus_;
    Date pickupDate_;
    Money pricePerPassenger_;
};
