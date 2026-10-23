#pragma once

#include <string>

#include "Hotel.hpp"
#include "Money.hpp"
#include "Tour.hpp"

/**
 * @brief Представляет пляжный тур и хранит сведения о пляже, отеле и включённом снорклинге.
 */
class BeachTour : public Tour {
public:
    /**
     * @brief Создаёт объект класса BeachTour и инициализирует его исходные данные.
     * @param base базовый тур.
     * @param beachName название пляжа.
     * @param hotel отель.
     * @param snorkelingIncluded признак включённого снорклинга.
     */
    BeachTour(Tour base, std::string beachName, HotelPtr hotel, bool snorkelingIncluded);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `beachName_`.
     */
    const std::string& beachName() const { return beachName_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `hotel_`.
     */
    const HotelPtr& hotel() const { return hotel_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `snorkelingIncluded_`.
     */
    bool snorkelingIncluded() const { return snorkelingIncluded_; }
    /**
     * @brief Проверяет наличие признака «hotel with stars» у объекта.
     * @param minimumStars минимальное количество звёзд.
     * @return Результат проверки условия.
     */
    bool hasHotelWithStars(int minimumStars) const;
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
    std::string beachName_;
    HotelPtr hotel_;
    bool snorkelingIncluded_;
};
