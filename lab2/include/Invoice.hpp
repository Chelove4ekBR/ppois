#pragma once

#include <string>

#include "Booking.hpp"
#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Представляет счёт по бронированию с датами выставления и оплаты.
 */
class Invoice {
public:
    /**
     * @brief Создаёт объект класса Invoice и инициализирует его исходные данные.
     * @param number номер объекта.
     * @param booking бронирование.
     * @param issueDate дата выставления счёта.
     * @param dueDate срок оплаты.
     */
    Invoice(int number, BookingPtr booking, Date issueDate, Date dueDate);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `number_`.
     */
    int number() const { return number_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `booking_`.
     */
    const BookingPtr& booking() const { return booking_; }
    /**
     * @brief Возвращает дату выставления счёта.
     * @return Ссылка на сохранённый атрибут `issueDate_`.
     */
    const Date& issueDate() const { return issueDate_; }
    /**
     * @brief Возвращает срок оплаты счёта.
     * @return Ссылка на сохранённый атрибут `dueDate_`.
     */
    const Date& dueDate() const { return dueDate_; }
    /**
     * @brief Рассчитывает сумму, которая ещё подлежит оплате.
     * @return Рассчитанная денежная сумма.
     */
    Money amountDue() const;
    /**
     * @brief Проверяет, просрочен ли платёж на указанную дату.
     * @param today дата, на которую выполняется проверка.
     * @return Результат проверки условия.
     */
    bool isOverdue(const Date& today) const;
    /**
     * @brief Рассчитывает количество дней просрочки на указанную дату.
     * @param today дата, на которую выполняется проверка.
     * @return Рассчитанное числовое значение.
     */
    int daysOverdue(const Date& today) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    int number_;
    BookingPtr booking_;
    Date issueDate_;
    Date dueDate_;
};
