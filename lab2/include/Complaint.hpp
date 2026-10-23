#pragma once

#include <string>

#include "Booking.hpp"
#include "Customer.hpp"

class Complaint {
public:
    Complaint(int id, CustomerPtr customer, BookingPtr booking, std::string text);
    int id() const { return id_; }
    const CustomerPtr& customer() const { return customer_; }
    const BookingPtr& booking() const { return booking_; }
    const std::string& text() const { return text_; }
    bool resolved() const { return resolved_; }
    const std::string& resolution() const { return resolution_; }
    bool isOpen() const;
    void resolve(const std::string& resolutionText);
    std::string describe() const;

private:
    int id_;
    CustomerPtr customer_;
    BookingPtr booking_;
    std::string text_;
    bool resolved_;
    std::string resolution_;
};
