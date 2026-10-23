#include "Room.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

Room::Room(int number, int capacity, Money pricePerNight)
    : number_(number), capacity_(capacity), pricePerNight_(std::move(pricePerNight)) {}

bool Room::isAvailable(const DateRange &period) const
{
    return std::none_of(bookedPeriods_.begin(), bookedPeriods_.end(),
                        [&](const DateRange &booked)
                        { return booked.overlaps(period); });
}

bool Room::fits(int guests) const
{
    return guests <= capacity_;
}

void Room::reserve(const DateRange &period)
{
    if (!isAvailable(period))
    {
        throw RoomUnavailableException("Room " + std::to_string(number_) + " is booked during " + period.toString());
    }
    bookedPeriods_.push_back(period);
}

void Room::cancelReservation(const DateRange &period)
{
    bookedPeriods_.erase(std::remove_if(bookedPeriods_.begin(), bookedPeriods_.end(),
                                        [&](const DateRange &booked)
                                        {
                                            return booked.start().isSameDay(period.start()) &&
                                                   booked.end().isSameDay(period.end());
                                        }),
                         bookedPeriods_.end());
}

Money Room::costFor(const DateRange &period) const
{
    return pricePerNight_.times(period.lengthInDays());
}
