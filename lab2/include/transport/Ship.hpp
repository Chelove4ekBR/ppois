#pragma once

#include <memory>
#include <string>

/**
 * @brief Описывает судно, количество кают и палуб, а также пассажировместимость.
 */
class Ship {
public:
    /**
     * @brief Создаёт объект класса Ship и инициализирует его исходные данные.
     * @param name название объекта.
     * @param cabins значение параметра «cabins».
     * @param decks значение параметра «decks».
     * @param passengerCapacity значение параметра «passenger capacity».
     */
    Ship(std::string name, int cabins, int decks, int passengerCapacity);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `cabins_`.
     */
    int cabins() const { return cabins_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `decks_`.
     */
    int decks() const { return decks_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `passengerCapacity_`.
     */
    int passengerCapacity() const { return passengerCapacity_; }
    /**
     * @brief Рассчитывает среднее количество кают на палубу.
     * @return Рассчитанное числовое значение.
     */
    int cabinsPerDeck() const;
    /**
     * @brief Проверяет, может ли судно разместить указанное число пассажиров.
     * @param passengers количество пассажиров.
     * @return Результат проверки условия.
     */
    bool canHost(int passengers) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    std::string name_;
    int cabins_;
    int decks_;
    int passengerCapacity_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Ship`.
 */
using ShipPtr = std::shared_ptr<Ship>;
