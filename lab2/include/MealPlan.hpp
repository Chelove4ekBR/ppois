#pragma once

#include <string>

#include "Money.hpp"

/**
 * @brief Описывает план питания, стоимость на человека в день и количество приёмов пищи.
 */
class MealPlan {
public:
    /**
     * @brief Создаёт объект класса MealPlan и инициализирует его исходные данные.
     * @param name название объекта.
     * @param pricePerPersonPerDay значение параметра «price per person per day».
     * @param mealsPerDay значение параметра «meals per day».
     */
    MealPlan(std::string name, Money pricePerPersonPerDay, int mealsPerDay);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `pricePerPersonPerDay_`.
     */
    const Money& pricePerPersonPerDay() const { return pricePerPersonPerDay_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `mealsPerDay_`.
     */
    int mealsPerDay() const { return mealsPerDay_; }
    /**
     * @brief Рассчитывает стоимость с учётом заданного количества человек или дней.
     * @param guests количество гостей.
     * @param nights значение параметра «nights».
     * @return Рассчитанная денежная сумма.
     */
    Money costFor(int guests, int nights) const;
    /**
     * @brief Проверяет условие «full board» для объекта.
     * @return Результат проверки условия.
     */
    bool isFullBoard() const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    std::string name_;
    Money pricePerPersonPerDay_;
    int mealsPerDay_;
};
