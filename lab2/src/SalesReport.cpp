#include "SalesReport.hpp"

#include <algorithm>
#include <map>
#include <utility>

#include "Exceptions.hpp"

SalesReport::SalesReport(std::string title, const DateRange &period) : title_(std::move(title)), period_(period) {}

bool SalesReport::add(const BookingPtr &booking)
{
    if (!period_.contains(booking->createdOn()))
    {
        return false;
    }
    bookings_.push_back(booking);
    return true;
}

std::size_t SalesReport::bookingCount() const
{
    return bookings_.size();
}

std::size_t SalesReport::cancelledCount() const
{
    return static_cast<std::size_t>(std::count_if(bookings_.begin(), bookings_.end(), [](const BookingPtr &booking)
                                                  { return booking->status() == BookingStatus::Cancelled; }));
}

Money SalesReport::totalRevenue() const
{
    Money revenue;
    for (const BookingPtr &booking : bookings_)
    {
        revenue = booking->status() == BookingStatus::Cancelled ? revenue : revenue.plus(booking->totalPaid());
    }
    return revenue;
}

Money SalesReport::averageBookingValue() const
{
    if (bookings_.empty())
    {
        return Money();
    }
    return totalRevenue().dividedBy(static_cast<int>(bookings_.size()));
}

std::string SalesReport::topTourName() const
{
    if (bookings_.empty())
    {
        throw TravelException("Report " + title_ + " has no bookings");
    }
    std::map<std::string, Money> revenueByTour;
    for (const BookingPtr &booking : bookings_)
    {
        const std::string &tourName = booking->departure()->tour()->name();
        const auto found = revenueByTour.find(tourName);
        const Money current = found == revenueByTour.end() ? Money() : found->second;
        revenueByTour.insert_or_assign(tourName, current.plus(booking->totalPaid()));
    }
    return std::max_element(revenueByTour.begin(), revenueByTour.end(), [](const auto &left, const auto &right)
                            { return right.second.isGreaterThan(left.second); })
        ->first;
}
