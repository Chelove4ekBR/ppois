#include "Tour.hpp"

#include <utility>

#include "Exceptions.hpp"

Tour::Tour(int id, std::string name, Destination destination, int durationDays, Money basePrice, int capacity,
           Itinerary itinerary)
    : id_(id),
      name_(std::move(name)),
      destination_(std::move(destination)),
      durationDays_(durationDays),
      basePrice_(std::move(basePrice)),
      capacity_(capacity),
      itinerary_(std::move(itinerary)) {
    if (durationDays_ <= 0 || capacity_ <= 0) {
        throw TravelException("Tour duration and capacity must be positive");
    }
}

Money Tour::pricePerPerson() const {
    return basePrice_;
}

Money Tour::priceFor(int participants) const {
    return pricePerPerson().times(participants);
}

bool Tour::hasCapacityFor(int participants) const {
    return participants <= capacity_;
}

bool Tour::isSuitableForSeason(const Date& date) const {
    return destination_.isBestSeason(date);
}

std::string Tour::describe() const {
    return name_ + " in " + destination_.city().label() + ", " + std::to_string(durationDays_) + " days, " +
           pricePerPerson().toString();
}
