#pragma once

#include <string>

#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Поддерживаемые способы оплаты.
 */
enum class PaymentMethod {
    Card, /**< Оплата банковской картой. */
    Cash, /**< Оплата наличными. */
    BankTransfer, /**< Оплата банковским переводом. */
    Voucher /**< Оплата ваучером. */
};

/**
 * @brief Преобразует перечисление «способ оплаты» в строку.
 * @param method значение перечисления.
 * @return Строковое представление значения.
 */
std::string toString(PaymentMethod method);

/**
 * @brief Представляет платёж с суммой, способом оплаты и датой проведения.
 */
class Payment {
public:
    /**
     * @brief Создаёт объект класса Payment и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param amount денежная сумма.
     * @param method способ операции или оплаты.
     * @param date дата.
     */
    Payment(int id, Money amount, PaymentMethod method, Date date);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает денежную сумму объекта.
     * @return Ссылка на сохранённый атрибут `amount_`.
     */
    const Money& amount() const { return amount_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `method_`.
     */
    PaymentMethod method() const { return method_; }
    /**
     * @brief Возвращает дату операции.
     * @return Ссылка на сохранённый атрибут `date_`.
     */
    const Date& date() const { return date_; }
    /**
     * @brief Проверяет, выполнен ли платёж наличными.
     * @return Результат проверки условия.
     */
    bool isCash() const;
    /**
     * @brief Проверяет, произошла ли операция не позднее указанного срока.
     * @param deadline значение параметра «deadline».
     * @return Результат проверки условия.
     */
    bool isOnOrBefore(const Date& deadline) const;
    /**
     * @brief Рассчитывает часть платежа для возврата по заданному проценту.
     * @param percent процент.
     * @return Рассчитанная денежная сумма.
     */
    Money refundPortion(int percent) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    int id_;
    Money amount_;
    PaymentMethod method_;
    Date date_;
};
