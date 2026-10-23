#pragma once

#include <string>

#include "Money.hpp"
#include "Tour.hpp"

/**
 * @brief Представляет горнолыжный тур с курортом, стоимостью ски-пасса и уровнем сложности.
 */
class SkiTour : public Tour {
public:
    /**
     * @brief Создаёт объект класса SkiTour и инициализирует его исходные данные.
     * @param base базовый тур.
     * @param resortName название горнолыжного курорта.
     * @param skiPassPricePerDay цена ски-пасса за день.
     * @param difficulty уровень сложности.
     */
    SkiTour(Tour base, std::string resortName, Money skiPassPricePerDay, int difficulty);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `resortName_`.
     */
    const std::string& resortName() const { return resortName_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `skiPassPricePerDay_`.
     */
    const Money& skiPassPricePerDay() const { return skiPassPricePerDay_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `difficulty_`.
     */
    int difficulty() const { return difficulty_; }
    /**
     * @brief Рассчитывает стоимость ски-пасса для тура.
     * @return Рассчитанная денежная сумма.
     */
    Money skiPassCost() const;
    /**
     * @brief Проверяет, подходит ли уровень сложности тура начинающим.
     * @return Результат проверки условия.
     */
    bool isForBeginners() const;
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
    std::string resortName_;
    Money skiPassPricePerDay_;
    int difficulty_;
};
