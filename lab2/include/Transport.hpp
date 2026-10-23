#pragma once

#include <memory>
#include <string>

#include "Money.hpp"

/**
 * @brief Базовый класс транспортного средства с перевозчиком, вместимостью, стоимостью и учётом проданных мест.
 */
class Transport {
public:
    /**
     * @brief Создаёт объект класса Transport и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param carrierName название перевозчика.
     * @param capacity вместимость транспорта.
     * @param pricePerSeat цена за место.
     */
    Transport(int id, std::string carrierName, int capacity, Money pricePerSeat);
    /**
     * @brief Освобождает ресурсы базового объекта Transport при уничтожении.
     */
    virtual ~Transport() = default;
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `carrierName_`.
     */
    const std::string& carrierName() const { return carrierName_; }
    /**
     * @brief Возвращает вместимость объекта.
     * @return Значение сохранённого атрибута `capacity_`.
     */
    int capacity() const { return capacity_; }
    /**
     * @brief Возвращает количество проданных мест.
     * @return Значение сохранённого атрибута `soldSeats_`.
     */
    int soldSeats() const { return soldSeats_; }
    /**
     * @brief Возвращает цену за одно место.
     * @return Ссылка на сохранённый атрибут `pricePerSeat_`.
     */
    const Money& pricePerSeat() const { return pricePerSeat_; }
    /**
     * @brief Возвращает количество свободных мест.
     * @return Рассчитанное числовое значение.
     */
    int freeSeats() const;
    /**
     * @brief Проверяет, достигнута ли максимальная вместимость.
     * @return Результат проверки условия.
     */
    bool isFull() const;
    /**
     * @brief Регистрирует продажу одного места и возвращает его номер.
     * @return Рассчитанное числовое значение.
     */
    int sellSeat();
    /**
     * @brief Регистрирует возврат ранее проданного места.
     */
    void refundSeat();
    /**
     * @brief Возвращает вычисленное значение на основе данных объекта.
     * @return Рассчитанная денежная сумма.
     */
    virtual Money ticketPrice() const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    virtual std::string describe() const;

private:
    int id_;
    std::string carrierName_;
    int capacity_;
    int soldSeats_;
    Money pricePerSeat_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Transport`.
 */
using TransportPtr = std::shared_ptr<Transport>;
