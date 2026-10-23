#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

/**
 * @brief Представляет скидку с промокодом, сроком действия и ограничением на количество применений.
 */
class Discount {
public:
    /**
     * @brief Создаёт объект класса Discount и инициализирует его исходные данные.
     * @param code код.
     * @param percent процент.
     * @param validity период действия.
     * @param usageLimit лимит применений скидки.
     */
    Discount(std::string code, int percent, DateRange validity, int usageLimit);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `code_`.
     */
    const std::string& code() const { return code_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `percent_`.
     */
    int percent() const { return percent_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `validity_`.
     */
    const DateRange& validity() const { return validity_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `usageLimit_`.
     */
    int usageLimit() const { return usageLimit_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `timesUsed_`.
     */
    int timesUsed() const { return timesUsed_; }
    /**
     * @brief Проверяет действие объекта на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isValidOn(const Date& date) const;
    /**
     * @brief Применяет скидку или другую предусмотренную объектом операцию.
     * @param price цена.
     * @param date дата.
     * @return Рассчитанная денежная сумма.
     */
    Money apply(const Money& price, const Date& date);
    /**
     * @brief Возвращает количество оставшихся применений скидки.
     * @return Рассчитанное числовое значение.
     */
    int remainingUses() const;

private:
    std::string code_;
    int percent_;
    DateRange validity_;
    int usageLimit_;
    int timesUsed_;
};
