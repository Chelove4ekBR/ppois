#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Address.hpp"
#include "DateRange.hpp"
#include "Money.hpp"
#include "Room.hpp"

class Hotel {
public:
    Hotel(std::string name, int stars, Address address);
    const std::string& name() const { return name_; }
    int stars() const { return stars_; }
    const Address& address() const { return address_; }
    const std::vector<RoomPtr>& rooms() const { return rooms_; }
    RoomPtr addRoom(int number, int capacity, const Money& pricePerNight);
    RoomPtr findAvailableRoom(const DateRange& period, int guests) const;
    std::size_t countAvailable(const DateRange& period) const;
    Money averagePrice() const;
    bool isLuxury() const;

private:
    std::string name_;
    int stars_;
    Address address_;
    std::vector<RoomPtr> rooms_;
};

using HotelPtr = std::shared_ptr<Hotel>;
