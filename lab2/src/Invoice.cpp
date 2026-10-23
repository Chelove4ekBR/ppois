#include "Invoice.hpp"

#include <utility>

Invoice::Invoice(int number, BookingPtr booking, Date issueDate, Date dueDate)
    : number_(number), booking_(std::move(booking)), issueDate_(issueDate), dueDate_(dueDate) {}

Money Invoice::amountDue() const {
    return booking_->amountDue();
}

bool Invoice::isOverdue(const Date& today) const {
    return dueDate_.isBefore(today) && amountDue().cents() > 0;
}

int Invoice::daysOverdue(const Date& today) const {
    return isOverdue(today) ? dueDate_.daysUntil(today) : 0;
}

std::string Invoice::describe() const {
    return "Invoice #" + std::to_string(number_) + " for booking " + std::to_string(booking_->id()) + ", due " +
           dueDate_.toString() + ", amount " + amountDue().toString();
}
