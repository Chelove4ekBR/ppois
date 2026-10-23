#pragma once

#include <string>

#include "Customer.hpp"
#include "Money.hpp"
#include "Transport.hpp"

/**
 * @brief Представляет билет на транспорт для конкретного пассажира и фиксирует номер места и цену.
 */
class TransportTicket {
public:
    /**
     * @brief Создаёт объект класса TransportTicket и инициализирует его исходные данные.
     * @param transport значение параметра «transport».
     * @param passenger значение параметра «passenger».
     */
    TransportTicket(TransportPtr transport, CustomerPtr passenger);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `transport_`.
     */
    const TransportPtr& transport() const { return transport_; }
    /**
     * @brief Возвращает пассажира, связанного с билетом.
     * @return Ссылка на сохранённый атрибут `passenger_`.
     */
    const CustomerPtr& passenger() const { return passenger_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `seatNumber_`.
     */
    int seatNumber() const { return seatNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `price_`.
     */
    const Money& price() const { return price_; }
    /**
     * @brief Проверяет условие «for passenger» для объекта.
     * @param customerId идентификатор клиента.
     * @return Результат проверки условия.
     */
    bool isForPassenger(int customerId) const;
    /**
     * @brief Отменяет объект или связанную с ним операцию.
     */
    void cancel();
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    TransportPtr transport_;
    CustomerPtr passenger_;
    int seatNumber_;
    Money price_;
};
