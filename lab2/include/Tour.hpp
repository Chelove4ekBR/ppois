#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Destination.hpp"
#include "Itinerary.hpp"
#include "Money.hpp"

/**
 * @brief Представляет базовую модель туристического тура: направление, длительность, цену, вместимость и маршрут.
 */
class Tour {
public:
    /**
     * @brief Создаёт объект класса Tour и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param name название объекта.
     * @param destination значение параметра «destination».
     * @param durationDays значение параметра «duration days».
     * @param basePrice значение параметра «base price».
     * @param capacity вместимость транспорта.
     * @param itinerary значение параметра «itinerary».
     */
    Tour(int id, std::string name, Destination destination, int durationDays, Money basePrice, int capacity,
         Itinerary itinerary);
    /**
     * @brief Освобождает ресурсы базового объекта Tour при уничтожении.
     */
    virtual ~Tour() = default;
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `destination_`.
     */
    const Destination& destination() const { return destination_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `durationDays_`.
     */
    int durationDays() const { return durationDays_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `basePrice_`.
     */
    const Money& basePrice() const { return basePrice_; }
    /**
     * @brief Возвращает вместимость объекта.
     * @return Значение сохранённого атрибута `capacity_`.
     */
    int capacity() const { return capacity_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `itinerary_`.
     */
    Itinerary& itinerary() { return itinerary_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `itinerary_`.
     */
    const Itinerary& itinerary() const { return itinerary_; }
    /**
     * @brief Возвращает цену на одного человека.
     * @return Рассчитанная денежная сумма.
     */
    virtual Money pricePerPerson() const;
    /**
     * @brief Рассчитывает стоимость для указанного количества участников.
     * @param participants количество участников.
     * @return Рассчитанная денежная сумма.
     */
    Money priceFor(int participants) const;
    /**
     * @brief Проверяет, достаточно ли вместимости для указанного количества участников.
     * @param participants количество участников.
     * @return Результат проверки условия.
     */
    bool hasCapacityFor(int participants) const;
    /**
     * @brief Проверяет, подходит ли дата для проведения тура.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isSuitableForSeason(const Date& date) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    virtual std::string describe() const;

private:
    int id_;
    std::string name_;
    Destination destination_;
    int durationDays_;
    Money basePrice_;
    int capacity_;
    Itinerary itinerary_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Tour`.
 */
using TourPtr = std::shared_ptr<Tour>;
