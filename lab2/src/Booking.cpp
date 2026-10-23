#include "Booking.hpp"

#include <utility>

#include "Exceptions.hpp"

std::string toString(BookingStatus status) {
    switch (status) {
        case BookingStatus::Pending:
            return "Pending";
        case BookingStatus::Confirmed:
            return "Confirmed";
        case BookingStatus::Paid:
            return "Paid";
        case BookingStatus::Cancelled:
            return "Cancelled";
        case BookingStatus::Completed:
            return "Completed";
    }
    return "Unknown";
}

Booking::Booking(int id, CustomerPtr customer, DeparturePtr departure, int participants, Date createdOn)
    : id_(id),
      customer_(std::move(customer)),
      departure_(std::move(departure)),
      participants_(participants),
      status_(BookingStatus::Pending),
      total_(),
      createdOn_(createdOn) {
    if (participants_ <= 0) {
        throw TravelException("Booking needs at least one participant");
    }
    departure_->reserveSeats(participants_);
    total_ = departure_->tour()->priceFor(participants_);
}

void Booking::requireStatus(BookingStatus expected, const std::string& action) const {
    if (status_ != expected) {
        throw InvalidBookingStateException("Cannot " + action + " booking " + std::to_string(id_) + " in status " +
                                           toString(status_));
    }
}

void Booking::confirm() {
    requireStatus(BookingStatus::Pending, "confirm");
    status_ = BookingStatus::Confirmed;
}

void Booking::applyDiscount(Discount& discount, const Date& date) {
    const bool openForDiscount = status_ == BookingStatus::Pending || status_ == BookingStatus::Confirmed;
    if (!openForDiscount || !payments_.empty()) {
        throw InvalidBookingStateException("Discount cannot be applied to booking " + std::to_string(id_));
    }
    total_ = discount.apply(total_, date);
}

void Booking::attachInsurance(const InsurancePolicyPtr& policy) {
    if (policy->insured()->customerId() != customer_->customerId()) {
        throw InsuranceException("Policy " + policy->policyNumber() + " belongs to another customer");
    }
    insurance_ = policy;
}

void Booking::addPayment(const Payment& payment) {
    if (status_ != BookingStatus::Confirmed && status_ != BookingStatus::Paid) {
        throw InvalidBookingStateException("Booking " + std::to_string(id_) + " does not accept payments in status " +
                                           toString(status_));
    }
    if (payment.amount().isGreaterThan(amountDue())) {
        throw PaymentException("Payment exceeds amount due " + amountDue().toString());
    }
    payments_.push_back(payment);
    status_ = isFullyPaid() ? BookingStatus::Paid : status_;
}

Money Booking::totalPaid() const {
    Money paid;
    for (const Payment& payment : payments_) {
        paid = paid.plus(payment.amount());
    }
    return paid;
}

Money Booking::amountDue() const {
    return total_.minus(totalPaid());
}

bool Booking::isFullyPaid() const {
    return amountDue().cents() <= 0;
}

Money Booking::cancel(const CancellationPolicy& policy, const Date& cancelDate) {
    if (status_ == BookingStatus::Cancelled || status_ == BookingStatus::Completed) {
        throw InvalidBookingStateException("Booking " + std::to_string(id_) + " is already " + toString(status_));
    }
    const Money paid = totalPaid();
    const Money penalty = policy.penaltyFor(paid, cancelDate, departure_->period().start());
    departure_->releaseSeats(participants_);
    status_ = BookingStatus::Cancelled;
    return paid.minus(penalty);
}

void Booking::complete(const Date& today) {
    requireStatus(BookingStatus::Paid, "complete");
    if (!departure_->isFinished(today)) {
        throw InvalidBookingStateException("Trip of booking " + std::to_string(id_) + " has not finished yet");
    }
    status_ = BookingStatus::Completed;
}
