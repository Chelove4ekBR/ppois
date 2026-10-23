#pragma once

#include <string>
#include <vector>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"
#include "Tour.hpp"
#include "TourDeparture.hpp"

/**
 * @brief Хранит каталог туров и отправлений, выполняет поиск и фильтрацию доступных вариантов.
 */
class TourCatalog {
public:
    /**
     * @brief Создаёт объект класса TourCatalog и инициализирует его исходные данные.
     */
    TourCatalog();
    /**
     * @brief Возвращает список туров.
     * @return Ссылка на сохранённый атрибут `tours_`.
     */
    const std::vector<TourPtr>& tours() const { return tours_; }
    /**
     * @brief Возвращает список отправлений.
     * @return Ссылка на сохранённый атрибут `departures_`.
     */
    const std::vector<DeparturePtr>& departures() const { return departures_; }
    /**
     * @brief Добавляет тур в каталог.
     * @param tour тур.
     */
    void addTour(const TourPtr& tour);
    /**
     * @brief Добавляет отправление тура в каталог.
     * @param tour тур.
     * @param period период дат.
     * @return Объект или значение, полученное в результате операции.
     */
    DeparturePtr addDeparture(const TourPtr& tour, const DateRange& period);
    /**
     * @brief Ищет тур по идентификатору.
     * @param tourId идентификатор тура.
     * @return Объект или значение, полученное в результате операции.
     */
    TourPtr findTour(int tourId) const;
    /**
     * @brief Ищет отправление по идентификатору.
     * @param departureId идентификатор отправления.
     * @return Объект или значение, полученное в результате операции.
     */
    DeparturePtr findDeparture(int departureId) const;
    /**
     * @brief Возвращает туры, соответствующие указанному городу.
     * @param cityName название города.
     * @return Текстовое представление или строковое значение.
     */
    std::vector<TourPtr> findByCity(const std::string& cityName) const;
    /**
     * @brief Возвращает туры, цена которых не превышает заданный предел.
     * @param maxPrice максимальная цена.
     * @return Объект или значение, полученное в результате операции.
     */
    std::vector<TourPtr> findAffordable(const Money& maxPrice) const;
    /**
     * @brief Возвращает доступные отправления, начиная с указанной даты.
     * @param from адрес или начальное значение.
     * @return Объект или значение, полученное в результате операции.
     */
    std::vector<DeparturePtr> availableDepartures(const Date& from) const;

private:
    std::vector<TourPtr> tours_;
    std::vector<DeparturePtr> departures_;
};
