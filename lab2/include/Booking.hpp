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

enum class BookingStatus { Pending, Confirmed, Paid, Cancelled, Completed };

std::string toString(BookingStatus status);

class Booking {
public:
    Booking(int id, CustomerPtr customer, DeparturePtr departure, int participants, Date createdOn);
    int id() const { return id_; }
    const CustomerPtr& customer() const { return customer_; }
    const DeparturePtr& departure() const { return departure_; }
    int participants() const { return participants_; }
    BookingStatus status() const { return status_; }
    const Money& total() const { return total_; }
    const std::vector<Payment>& payments() const { return payments_; }
    const InsurancePolicyPtr& insurance() const { return insurance_; }
    const Date& createdOn() const { return createdOn_; }
    void confirm();
    void applyDiscount(Discount& discount, const Date& date);
    void attachInsurance(const InsurancePolicyPtr& policy);
    void addPayment(const Payment& payment);
    Money totalPaid() const;
    Money amountDue() const;
    bool isFullyPaid() const;
    Money cancel(const CancellationPolicy& policy, const Date& cancelDate);
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

using BookingPtr = std::shared_ptr<Booking>;
