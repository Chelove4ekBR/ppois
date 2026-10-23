#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CancellationPolicy.hpp"
#include "Customer.hpp"
#include "Date.hpp"
#include "Discount.hpp"
#include "InsurancePolicy.hpp"
#include "Money.hpp"
#include "Payment.hpp"
#include "TourDeparture.hpp"

/**
 * @brief Состояния жизненного цикла бронирования.
 */
enum class BookingStatus {
    Pending, /**< Бронирование создано и ожидает подтверждения. */
    Confirmed, /**< Бронирование подтверждено. */
    Paid, /**< Бронирование полностью оплачено. */
    Cancelled, /**< Бронирование отменено. */
    Completed /**< Поездка по бронированию завершена. */
};

/**
 * @brief Преобразует перечисление «статус бронирования» в строку.
 * @param status значение перечисления.
 * @return Строковое представление значения.
 */
std::string toString(BookingStatus status);

/**
 * @brief Представляет бронирование тура, управляет его состоянием, платежами, скидкой, страховкой и отменой.
 */
class Booking {
public:
    /**
     * @brief Создаёт объект класса Booking и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param customer клиент.
     * @param departure отправление тура.
     * @param participants количество участников.
     * @param createdOn дата создания.
     */
    Booking(int id, CustomerPtr customer, DeparturePtr departure, int participants, Date createdOn);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает клиента, связанного с объектом.
     * @return Ссылка на сохранённый атрибут `customer_`.
     */
    const CustomerPtr& customer() const { return customer_; }
    /**
     * @brief Возвращает отправление тура, связанное с бронированием.
     * @return Ссылка на сохранённый атрибут `departure_`.
     */
    const DeparturePtr& departure() const { return departure_; }
    /**
     * @brief Возвращает число участников бронирования.
     * @return Значение сохранённого атрибута `participants_`.
     */
    int participants() const { return participants_; }
    /**
     * @brief Возвращает текущее состояние объекта.
     * @return Значение сохранённого атрибута `status_`.
     */
    BookingStatus status() const { return status_; }
    /**
     * @brief Возвращает итоговую сумму.
     * @return Ссылка на сохранённый атрибут `total_`.
     */
    const Money& total() const { return total_; }
    /**
     * @brief Возвращает список платежей.
     * @return Ссылка на сохранённый атрибут `payments_`.
     */
    const std::vector<Payment>& payments() const { return payments_; }
    /**
     * @brief Возвращает прикреплённый страховой полис.
     * @return Ссылка на сохранённый атрибут `insurance_`.
     */
    const InsurancePolicyPtr& insurance() const { return insurance_; }
    /**
     * @brief Возвращает дату создания объекта.
     * @return Ссылка на сохранённый атрибут `createdOn_`.
     */
    const Date& createdOn() const { return createdOn_; }
    /**
     * @brief Подтверждает бронирование.
     */
    void confirm();
    /**
     * @brief Применяет скидку к бронированию.
     * @param discount скидка.
     * @param date дата.
     */
    void applyDiscount(Discount& discount, const Date& date);
    /**
     * @brief Привязывает страховой полис к бронированию.
     * @param policy правила или полис.
     */
    void attachInsurance(const InsurancePolicyPtr& policy);
    /**
     * @brief Добавляет платёж к бронированию.
     * @param payment платёж.
     */
    void addPayment(const Payment& payment);
    /**
     * @brief Рассчитывает общую сумму платежей по объекту.
     * @return Рассчитанная денежная сумма.
     */
    Money totalPaid() const;
    /**
     * @brief Рассчитывает сумму, которая ещё подлежит оплате.
     * @return Рассчитанная денежная сумма.
     */
    Money amountDue() const;
    /**
     * @brief Проверяет, полностью ли оплачено бронирование.
     * @return Результат проверки условия.
     */
    bool isFullyPaid() const;
    /**
     * @brief Отменяет объект или связанную с ним операцию.
     * @param policy правила или полис.
     * @param cancelDate дата отмены.
     * @return Рассчитанная денежная сумма.
     */
    Money cancel(const CancellationPolicy& policy, const Date& cancelDate);
    /**
     * @brief Отмечает выполнение или завершение операции.
     * @param today дата, на которую выполняется проверка.
     */
    void complete(const Date& today);

private:
    void requireStatus(BookingStatus expected, const std::string& action) const;

    int id_;
    CustomerPtr customer_;
    DeparturePtr departure_;
    int participants_;
    BookingStatus status_;
    Money total_;
    std::vector<Payment> payments_;
    InsurancePolicyPtr insurance_;
    Date createdOn_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Booking`.
 */
using BookingPtr = std::shared_ptr<Booking>;
