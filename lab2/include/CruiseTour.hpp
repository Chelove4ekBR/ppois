#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "City.hpp"
#include "Money.hpp"
#include "Ship.hpp"
#include "Tour.hpp"

/**
 * @brief Представляет круизный тур с судном, категорией каюты и списком городов захода.
 */
class CruiseTour : public Tour {
public:
    /**
     * @brief Создаёт объект класса CruiseTour и инициализирует его исходные данные.
     * @param base базовый тур.
     * @param ship судно.
     * @param cabinCategory категория каюты.
     * @param ports города захода круизного судна.
     */
    CruiseTour(Tour base, ShipPtr ship, std::string cabinCategory, std::vector<City> ports);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `ship_`.
     */
    const ShipPtr& ship() const { return ship_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `cabinCategory_`.
     */
    const std::string& cabinCategory() const { return cabinCategory_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `ports_`.
     */
    const std::vector<City>& ports() const { return ports_; }
    /**
     * @brief Возвращает процентный множитель цены для категории каюты.
     * @return Рассчитанное числовое значение.
     */
    int cabinMultiplierPercent() const;
    /**
     * @brief Возвращает количество портов в маршруте круиза.
     * @return Рассчитанное числовое значение.
     */
    std::size_t portCount() const;
    /**
     * @brief Проверяет, включён ли указанный город в маршрут круиза.
     * @param cityName название города.
     * @return Результат проверки условия.
     */
    bool visitsCity(const std::string& cityName) const;
    /**
     * @brief Возвращает цену на одного человека.
     * @return Рассчитанная денежная сумма.
     */
    Money pricePerPerson() const override;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const override;

private:
    ShipPtr ship_;
    std::string cabinCategory_;
    std::vector<City> ports_;
};
