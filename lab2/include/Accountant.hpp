#pragma once

#include <string>

#include "Booking.hpp"
#include "Date.hpp"
#include "Employee.hpp"
#include "Invoice.hpp"
#include "Ledger.hpp"
#include "Money.hpp"
#include "Payment.hpp"
#include "Person.hpp"

/**
 * @brief Представляет сотрудника бухгалтерии туристической компании. Выписывает счета и регистрирует платежи и возвраты.
 */
class Accountant : public Employee {
public:
    /**
     * @brief Создаёт объект класса Accountant и инициализирует его исходные данные.
     * @param employeeId идентификатор сотрудника.
     * @param person персональные данные.
     * @param salary зарплата.
     * @param hireDate дата приёма на работу.
     * @param certificateNumber номер сертификата.
     */
    Accountant(int employeeId, Person person, Money salary, Date hireDate, std::string certificateNumber);
    /**
     * @brief Возвращает номер профессионального сертификата сотрудника.
     * @return Ссылка на сохранённый атрибут `certificateNumber_`.
     */
    const std::string& certificateNumber() const { return certificateNumber_; }
    /**
     * @brief Возвращает количество обработанных платежей.
     * @return Значение сохранённого атрибута `processedPayments_`.
     */
    int processedPayments() const { return processedPayments_; }
    /**
     * @brief Возвращает текущий счётчик выставленных счетов.
     * @return Значение сохранённого атрибута `invoiceCounter_`.
     */
    int invoiceCounter() const { return invoiceCounter_; }
    /**
     * @brief Оформляет счёт для бронирования.
     * @param booking бронирование.
     * @param issueDate дата выставления счёта.
     * @return Объект или значение, полученное в результате операции.
     */
    Invoice issueInvoice(const BookingPtr& booking, const Date& issueDate);
    /**
     * @brief Регистрирует платёж в финансовом журнале.
     * @param ledger финансовый журнал.
     * @param payment платёж.
     */
    void recordPayment(Ledger& ledger, const Payment& payment);
    /**
     * @brief Регистрирует возврат денежных средств в финансовом журнале.
     * @param ledger финансовый журнал.
     * @param amount денежная сумма.
     * @param date дата.
     */
    void recordRefund(Ledger& ledger, const Money& amount, const Date& date);
    /**
     * @brief Возвращает текстовое описание роли сотрудника.
     * @return Текстовое представление или строковое значение.
     */
    std::string describeRole() const override;

private:
    std::string certificateNumber_;
    int processedPayments_;
    int invoiceCounter_;
};
