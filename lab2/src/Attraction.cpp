#include "Attraction.hpp"

#include <utility>

Attraction::Attraction(std::string name, Money ticketPrice, int visitMinutes, bool indoor)
    : name_(std::move(name)), ticketPrice_(std::move(ticketPrice)), visitMinutes_(visitMinutes), indoor_(indoor) {}

bool Attraction::isFree() const {
    return ticketPrice_.cents() == 0;
}

Money Attraction::costFor(int visitors) const {
    return ticketPrice_.times(visitors);
}

bool Attraction::fitsInto(int availableMinutes) const {
    return visitMinutes_ <= availableMinutes;
}
