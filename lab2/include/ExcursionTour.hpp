#pragma once

#include <string>

#include "Money.hpp"
#include "Tour.hpp"
#include "TourGuide.hpp"

/**
 * @brief Представляет экскурсионный тур с параметрами экскурсионной программы.
 */
class ExcursionTour : public Tour {
public:
    /**
     * @brief Создаёт объект класса ExcursionTour и инициализирует его исходные данные.
     * @param base базовый тур.
     * @param guide значение параметра «guide».
     * @param language название языка.
     * @param walkingKm значение параметра «walking km».
     */
    ExcursionTour(Tour base, TourGuidePtr guide, std::string language, int walkingKm);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `guide_`.
     */
    const TourGuidePtr& guide() const { return guide_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `language_`.
     */
    const std::string& language() const { return language_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `walkingKm_`.
     */
    int walkingKm() const { return walkingKm_; }
    /**
     * @brief Проверяет, владеет ли гид указанным языком.
     * @return Результат проверки условия.
     */
    bool guideSpeaksLanguage() const;
    /**
     * @brief Проверяет условие «easy» для объекта.
     * @return Результат проверки условия.
     */
    bool isEasy() const;
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
    TourGuidePtr guide_;
    std::string language_;
    int walkingKm_;
};
