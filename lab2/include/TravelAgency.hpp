#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Accountant.hpp"
#include "Booking.hpp"
#include "CancellationPolicy.hpp"
#include "Complaint.hpp"
#include "Customer.hpp"
#include "Date.hpp"
#include "DateRange.hpp"
#include "Discount.hpp"
#include "Ledger.hpp"
#include "Money.hpp"
#include "Notification.hpp"
#include "Office.hpp"
#include "Payment.hpp"
#include "Review.hpp"
#include "SalesReport.hpp"
#include "TourCatalog.hpp"

/**
 * @brief Координирует работу туристического агентства: клиентов, каталог, бронирования, платежи, жалобы и отчётность.
 */
class TravelAgency {
public:
    /**
     * @brief Создаёт объект класса TravelAgency и инициализирует его исходные данные.
     * @param name название объекта.
     * @param policy правила или полис.
     * @param accountant значение параметра «accountant».
     */
    TravelAgency(std::string name, CancellationPolicy policy, std::shared_ptr<Accountant> accountant);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `policy_`.
     */
    const CancellationPolicy& policy() const { return policy_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `catalog_`.
     */
    TourCatalog& catalog() { return catalog_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `catalog_`.
     */
    const TourCatalog& catalog() const { return catalog_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `ledger_`.
     */
    const Ledger& ledger() const { return ledger_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `customers_`.
     */
    const std::vector<CustomerPtr>& customers() const { return customers_; }
    /**
     * @brief Возвращает список бронирований.
     * @return Ссылка на сохранённый атрибут `bookings_`.
     */
    const std::vector<BookingPtr>& bookings() const { return bookings_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `offices_`.
     */
    const std::vector<Office>& offices() const { return offices_; }
    /**
     * @brief Регистрирует клиента в агентстве.
     * @param customer клиент.
     */
    void registerCustomer(const CustomerPtr& customer);
    /**
     * @brief Добавляет офис в агентство.
     * @param office значение параметра «office».
     */
    void addOffice(const Office& office);
    /**
     * @brief Добавляет скидку в агентство.
     * @param discount скидка.
     */
    void addDiscount(const Discount& discount);
    /**
     * @brief Ищет клиента по идентификатору.
     * @param customerId идентификатор клиента.
     * @return Объект или значение, полученное в результате операции.
     */
    CustomerPtr findCustomer(int customerId) const;
    /**
     * @brief Ищет бронирование по идентификатору.
     * @param bookingId идентификатор бронирования.
     * @return Объект или значение, полученное в результате операции.
     */
    BookingPtr findBooking(int bookingId) const;
    /**
     * @brief Создаёт бронирование для клиента и отправления.
     * @param customerId идентификатор клиента.
     * @param departureId идентификатор отправления.
     * @param participants количество участников.
     * @param today дата, на которую выполняется проверка.
     * @return Объект или значение, полученное в результате операции.
     */
    BookingPtr createBooking(int customerId, int departureId, int participants, const Date& today);
    /**
     * @brief Применяет к бронированию скидку по промокоду.
     * @param bookingId идентификатор бронирования.
     * @param code код.
     * @param today дата, на которую выполняется проверка.
     */
    void applyDiscountCode(int bookingId, const std::string& code, const Date& today);
    /**
     * @brief Регистрирует платёж по бронированию.
     * @param bookingId идентификатор бронирования.
     * @param amount денежная сумма.
     * @param method способ операции или оплаты.
     * @param today дата, на которую выполняется проверка.
     */
    void payBooking(int bookingId, const Money& amount, PaymentMethod method, const Date& today);
    /**
     * @brief Отменяет бронирование и возвращает рассчитанную сумму возврата.
     * @param bookingId идентификатор бронирования.
     * @param today дата, на которую выполняется проверка.
     * @return Рассчитанная денежная сумма.
     */
    Money cancelBooking(int bookingId, const Date& today);
    /**
     * @brief Добавляет отзыв в систему агентства.
     * @param review значение параметра «review».
     */
    void addReview(const Review& review);
    /**
     * @brief Возвращает среднюю оценку объекта.
     * @param tourId идентификатор тура.
     * @return Рассчитанное числовое значение.
     */
    double averageRating(int tourId) const;
    /**
     * @brief Регистрирует жалобу клиента и возвращает её идентификатор.
     * @param customerId идентификатор клиента.
     * @param bookingId идентификатор бронирования.
     * @param text текст.
     * @return Рассчитанное числовое значение.
     */
    int fileComplaint(int customerId, int bookingId, const std::string& text);
    /**
     * @brief Сохраняет решение по жалобе в агентстве.
     * @param complaintId идентификатор жалобы.
     * @param resolution решение по жалобе.
     */
    void resolveComplaint(int complaintId, const std::string& resolution);
    /**
     * @brief Возвращает количество нерешённых жалоб.
     * @return Рассчитанное числовое значение.
     */
    std::size_t openComplaintCount() const;
    /**
     * @brief Возвращает количество непрочитанных уведомлений клиента.
     * @param customerId идентификатор клиента.
     * @return Рассчитанное числовое значение.
     */
    std::size_t unreadNotifications(int customerId) const;
    /**
     * @brief Формирует отчёт о продажах за указанный период.
     * @param title название или заголовок.
     * @param period период дат.
     * @return Текстовое представление или строковое значение.
     */
    SalesReport generateReport(const std::string& title, const DateRange& period) const;

private:
    void notify(const CustomerPtr& customer, const std::string& message, const Date& date);

    std::string name_;
    CancellationPolicy policy_;
    TourCatalog catalog_;
    std::shared_ptr<Accountant> accountant_;
    Ledger ledger_;
    std::vector<CustomerPtr> customers_;
    std::vector<BookingPtr> bookings_;
    std::vector<Office> offices_;
    std::vector<Discount> discounts_;
    std::vector<Complaint> complaints_;
    std::vector<Review> reviews_;
    std::vector<Notification> notifications_;
    int nextBookingId_;
    int nextPaymentId_;
};
