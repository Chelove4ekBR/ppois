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

class TravelAgency {
public:
    TravelAgency(std::string name, CancellationPolicy policy, std::shared_ptr<Accountant> accountant);
    const std::string& name() const { return name_; }
    const CancellationPolicy& policy() const { return policy_; }
    TourCatalog& catalog() { return catalog_; }
    const TourCatalog& catalog() const { return catalog_; }
    const Ledger& ledger() const { return ledger_; }
    const std::vector<CustomerPtr>& customers() const { return customers_; }
    const std::vector<BookingPtr>& bookings() const { return bookings_; }
    const std::vector<Office>& offices() const { return offices_; }
    void registerCustomer(const CustomerPtr& customer);
    void addOffice(const Office& office);
    void addDiscount(const Discount& discount);
    CustomerPtr findCustomer(int customerId) const;
    BookingPtr findBooking(int bookingId) const;
    BookingPtr createBooking(int customerId, int departureId, int participants, const Date& today);
    void applyDiscountCode(int bookingId, const std::string& code, const Date& today);
    void payBooking(int bookingId, const Money& amount, PaymentMethod method, const Date& today);
    Money cancelBooking(int bookingId, const Date& today);
    void addReview(const Review& review);
    double averageRating(int tourId) const;
    int fileComplaint(int customerId, int bookingId, const std::string& text);
    void resolveComplaint(int complaintId, const std::string& resolution);
    std::size_t openComplaintCount() const;
    std::size_t unreadNotifications(int customerId) const;
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
