#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Driver.hpp"
#include "Money.hpp"
#include "Transport.hpp"

/**
 * @brief Описывает автобус как вид транспорта, включая регистрационный номер, водителя и наличие туалета.
 */
class Bus : public Transport {
public:
    /**
     * @brief Создаёт объект класса Bus и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param carrierName название перевозчика.
     * @param capacity вместимость транспорта.
     * @param pricePerSeat цена за место.
     * @param plateNumber регистрационный номер автобуса.
     * @param driver значение параметра «driver».
     * @param hasToilet признак наличия туалета.
     */
    Bus(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string plateNumber, DriverPtr driver,
        bool hasToilet);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `plateNumber_`.
     */
    const std::string& plateNumber() const { return plateNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `driver_`.
     */
    const DriverPtr& driver() const { return driver_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `hasToilet_`.
     */
    bool hasToilet() const { return hasToilet_; }
    /**
     * @brief Проверяет, имеет ли водитель необходимые условия допуска на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isDriverQualified(const Date& date) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const override;

private:
    std::string plateNumber_;
    DriverPtr driver_;
    bool hasToilet_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Bus`.
 */
using BusPtr = std::shared_ptr<Bus>;
