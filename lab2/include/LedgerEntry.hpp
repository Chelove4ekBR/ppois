#pragma once

#include <string>

#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Представляет одну финансовую запись с описанием, суммой, датой и признаком дохода.
 */
class LedgerEntry {
public:
    /**
     * @brief Создаёт объект класса LedgerEntry и инициализирует его исходные данные.
     * @param description описание.
     * @param amount денежная сумма.
     * @param date дата.
     * @param income признак дохода.
     */
    LedgerEntry(std::string description, Money amount, Date date, bool income);
    /**
     * @brief Возвращает описание объекта.
     * @return Ссылка на сохранённый атрибут `description_`.
     */
    const std::string& description() const { return description_; }
    /**
     * @brief Возвращает денежную сумму объекта.
     * @return Ссылка на сохранённый атрибут `amount_`.
     */
    const Money& amount() const { return amount_; }
    /**
     * @brief Возвращает дату операции.
     * @return Ссылка на сохранённый атрибут `date_`.
     */
    const Date& date() const { return date_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `income_`.
     */
    bool income() const { return income_; }
    /**
     * @brief Рассчитывает соответствующее значение на основе данных объекта.
     * @return Рассчитанная денежная сумма.
     */
    Money signedAmount() const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    std::string description_;
    Money amount_;
    Date date_;
    bool income_;
};
