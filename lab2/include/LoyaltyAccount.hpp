#pragma once

#include <string>

#include "Money.hpp"

/**
 * @brief Хранит баллы клиента и общую сумму его расходов для определения уровня программы лояльности.
 */
class LoyaltyAccount {
public:
    /**
     * @brief Создаёт объект класса LoyaltyAccount и инициализирует его исходные данные.
     */
    LoyaltyAccount();
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `points_`.
     */
    int points() const { return points_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `lifetimeSpent_`.
     */
    const Money& lifetimeSpent() const { return lifetimeSpent_; }
    /**
     * @brief Начисляет баллы программы лояльности за расходы.
     * @param spent значение параметра «spent».
     */
    void earnPoints(const Money& spent);
    /**
     * @brief Списывает указанное количество баллов программы лояльности.
     * @param amount денежная сумма.
     */
    void redeemPoints(int amount);
    /**
     * @brief Возвращает вычисленное значение на основе данных объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string tier() const;
    /**
     * @brief Возвращает вычисленное значение на основе данных объекта.
     * @return Рассчитанное числовое значение.
     */
    int discountPercent() const;

private:
    int points_;
    Money lifetimeSpent_;
};
