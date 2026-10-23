#include "Destination.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

#include "Exceptions.hpp"

Destination::Destination(City city, std::string description, std::vector<unsigned> bestMonths)
    : city_(std::move(city)), description_(std::move(description)), bestMonths_(std::move(bestMonths)) {}

void Destination::addAttraction(const Attraction& attraction) {
    attractions_.push_back(attraction);
}

bool Destination::isBestSeason(const Date& date) const {
    return std::find(bestMonths_.begin(), bestMonths_.end(), date.month()) != bestMonths_.end();
}

Money Destination::totalAttractionCost() const {
    Money total;
    for (const Attraction& attraction : attractions_) {
        total = total.plus(attraction.ticketPrice());
    }
    return total;
}

const Attraction& Destination::cheapestAttraction() const {
    if (attractions_.empty()) {
        throw TravelException("Destination " + city_.name() + " has no attractions");
    }
    return *std::min_element(attractions_.begin(), attractions_.end(), [](const Attraction& left, const Attraction& right) {
        return right.ticketPrice().isGreaterThan(left.ticketPrice());
    });
}

std::vector<Attraction> Destination::attractionsWithin(int minutes) const {
    std::vector<Attraction> result;
    std::copy_if(attractions_.begin(), attractions_.end(), std::back_inserter(result),
                 [minutes](const Attraction& attraction) { return attraction.fitsInto(minutes); });
    return result;
}
