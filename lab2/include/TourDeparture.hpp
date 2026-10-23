#pragma once

#include <memory>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Tour.hpp"

class TourDeparture {
public:
    TourDeparture(int id, TourPtr tour, DateRange period);
    int id() const { return id_; }
    const TourPtr& tour() const { return tour_; }
    const DateRange& period() const { return period_; }
    int bookedSeats() const { return bookedSeats_; }
    int freeSeats() const;
    void reserveSeats(int seats);
    void releaseSeats(int seats);
    bool isSoldOut() const;
    bool startsWithin(const Date& today, int days) const;
    bool isFinished(const Date& today) const;

private:
    int id_;
    TourPtr tour_;
    DateRange period_;
    int bookedSeats_;
};

using DeparturePtr = std::shared_ptr<TourDeparture>;
