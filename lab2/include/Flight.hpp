#pragma once

#include <string>

#include "Airport.hpp"
#include "Date.hpp"
#include "Money.hpp"
#include "Transport.hpp"

/**
 * @brief Описывает авиарейс, аэропорты отправления и прибытия, дату вылета и включённый багаж.
 */
class Flight : public Transport {
public:
    /**
     * @brief Создаёт объект класса Flight и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param carrierName название перевозчика.
     * @param capacity вместимость транспорта.
     * @param pricePerSeat цена за место.
     * @param flightNumber номер рейса.
     * @param origin значение параметра «origin».
     * @param destination значение параметра «destination».
     * @param departureDate дата отправления.
     * @param luggageKgIncluded масса включённого багажа в килограммах.
     */
    Flight(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string flightNumber,
           Airport origin, Airport destination, Date departureDate, int luggageKgIncluded);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `flightNumber_`.
     */
    const std::string& flightNumber() const { return flightNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `origin_`.
     */
    const Airport& origin() const { return origin_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `destination_`.
     */
    const Airport& destination() const { return destination_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `departureDate_`.
     */
    const Date& departureDate() const { return departureDate_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `luggageKgIncluded_`.
     */
    int luggageKgIncluded() const { return luggageKgIncluded_; }
    /**
     * @brief Проверяет, является ли маршрут международным.
     * @return Результат проверки условия.
     */
    bool isInternational() const;
    /**
     * @brief Рассчитывает дополнительную плату за багаж сверх включённой нормы.
     * @param luggageKg масса багажа в килограммах.
     * @return Рассчитанная денежная сумма.
     */
    Money luggageFee(int luggageKg) const;
    /**
     * @brief Возвращает вычисленное значение на основе данных объекта.
     * @return Рассчитанная денежная сумма.
     */
    Money ticketPrice() const override;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const override;

private:
    std::string flightNumber_;
    Airport origin_;
    Airport destination_;
    Date departureDate_;
    int luggageKgIncluded_;
};
