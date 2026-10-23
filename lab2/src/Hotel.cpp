#include "Hotel.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr int kLuxuryStars = 5;

}

Hotel::Hotel(std::string name, int stars, Address address)
    : name_(std::move(name)), stars_(stars), address_(std::move(address)) {}

RoomPtr Hotel::addRoom(int number, int capacity, const Money& pricePerNight) {
    RoomPtr room = std::make_shared<Room>(number, capacity, pricePerNight);
    rooms_.push_back(room);
    return room;
}

RoomPtr Hotel::findAvailableRoom(const DateRange& period, int guests) const {
    for (const RoomPtr& room : rooms_) {
        if (room->fits(guests) && room->isAvailable(period)) {
            return room;
        }
    }
    throw RoomUnavailableException("No room for " + std::to_string(guests) + " guests in " + name_);
}

std::size_t Hotel::countAvailable(const DateRange& period) const {
    return static_cast<std::size_t>(std::count_if(
        rooms_.begin(), rooms_.end(), [&](const RoomPtr& room) { return room->isAvailable(period); }));
}

Money Hotel::averagePrice() const {
    if (rooms_.empty()) {
        return Money();
    }
    Money total;
    for (const RoomPtr& room : rooms_) {
        total = total.plus(room->pricePerNight());
    }
    return total.dividedBy(static_cast<int>(rooms_.size()));
}

bool Hotel::isLuxury() const {
    return stars_ >= kLuxuryStars;
}
