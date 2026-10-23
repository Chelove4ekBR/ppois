#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Booking.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

class SalesReport {
public:
    SalesReport(std::string title, DateRange period);
    const std::string& title() const { return title_; }
    const DateRange& period() const { return period_; }
    const std::vector<BookingPtr>& bookings() const { return bookings_; }
    bool add(const BookingPtr& booking);
    std::size_t bookingCount() const;
    std::size_t cancelledCount() const;
    Money totalRevenue() const;
    Money averageBookingValue() const;
    std::string topTourName() const;

private:
    std::string title_;
    DateRange period_;
    std::vector<BookingPtr> bookings_;
};
