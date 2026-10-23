#pragma once

#include <memory>
#include <vector>

#include "DateRange.hpp"
#include "Money.hpp"

class Room {
public:
    Room(int number, int capacity, Money pricePerNight);
    int number() const { return number_; }
    int capacity() const { return capacity_; }
    const Money& pricePerNight() const { return pricePerNight_; }
    const std::vector<DateRange>& bookedPeriods() const { return bookedPeriods_; }
    bool isAvailable(const DateRange& period) const;
    bool fits(int guests) const;
    void reserve(const DateRange& period);
    void cancelReservation(const DateRange& period);
    Money costFor(const DateRange& period) const;

private:
    int number_;
    int capacity_;
    Money pricePerNight_;
    std::vector<DateRange> bookedPeriods_;
};

using RoomPtr = std::shared_ptr<Room>;
