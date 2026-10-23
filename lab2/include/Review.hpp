#pragma once

#include <string>

#include "Customer.hpp"
#include "Tour.hpp"

class Review {
public:
    Review(CustomerPtr author, TourPtr tour, int rating, std::string text);
    const CustomerPtr& author() const { return author_; }
    const TourPtr& tour() const { return tour_; }
    int rating() const { return rating_; }
    const std::string& text() const { return text_; }
    bool isPositive() const;
    bool isByCustomer(int customerId) const;
    std::string summary() const;

private:
    CustomerPtr author_;
    TourPtr tour_;
    int rating_;
    std::string text_;
};
