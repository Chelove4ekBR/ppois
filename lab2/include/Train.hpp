#pragma once

#include <string>

#include "Money.hpp"
#include "Transport.hpp"

/**
 * @brief Описывает поезд как вид транспорта, включая номер, количество вагонов и наличие спальных мест.
 */
class Train : public Transport {
public:
    /**
     * @brief Создаёт объект класса Train и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param carrierName название перевозчика.
     * @param capacity вместимость транспорта.
     * @param pricePerSeat цена за место.
     * @param trainNumber номер поезда.
     * @param carriages количество вагонов.
     * @param hasSleeper признак наличия спальных мест.
     */
    Train(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string trainNumber, int carriages,
          bool hasSleeper);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `trainNumber_`.
     */
    const std::string& trainNumber() const { return trainNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `carriages_`.
     */
    int carriages() const { return carriages_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `hasSleeper_`.
     */
    bool hasSleeper() const { return hasSleeper_; }
    /**
     * @brief Рассчитывает количество мест на вагон.
     * @return Рассчитанное числовое значение.
     */
    int seatsPerCarriage() const;
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
    std::string trainNumber_;
    int carriages_;
    bool hasSleeper_;
};
