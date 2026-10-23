#pragma once

#include <string>

#include "Booking.hpp"
#include "Date.hpp"
#include "Money.hpp"

class Invoice {
public:
    Invoice(int number, BookingPtr booking, Date issueDate, Date dueDate);
    int number() const { return number_; }
    const BookingPtr& booking() const { return booking_; }
    const Date& issueDate() const { return issueDate_; }
    const Date& dueDate() const { return dueDate_; }
    Money amountDue() const;
    bool isOverdue(const Date& today) const;
    int daysOverdue(const Date& today) const;
    std::string describe() const;

private:
    int number_;
    BookingPtr booking_;
    Date issueDate_;
    Date dueDate_;
};
