#include "Itinerary.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

Itinerary::Itinerary(std::string title, City startCity) : title_(std::move(title)), startCity_(std::move(startCity)) {}

void Itinerary::addDay(const ItineraryDay& day) {
    if (day.dayNumber() != totalDays() + 1) {
        throw TravelException("Unexpected itinerary day number " + std::to_string(day.dayNumber()));
    }
    days_.push_back(day);
}

int Itinerary::totalDays() const {
    return static_cast<int>(days_.size());
}

Money Itinerary::totalCost() const {
    Money total;
    for (const ItineraryDay& day : days_) {
        total = total.plus(day.totalCost());
    }
    return total;
}

const ItineraryDay& Itinerary::busiestDay() const {
    if (days_.empty()) {
        throw TravelException("Itinerary " + title_ + " has no days");
    }
    return *std::max_element(days_.begin(), days_.end(), [](const ItineraryDay& left, const ItineraryDay& right) {
        return left.totalMinutes() < right.totalMinutes();
    });
}
