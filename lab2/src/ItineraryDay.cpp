#include "ItineraryDay.hpp"

#include <utility>

namespace {

constexpr int kMaxDayMinutes = 720;

}

ItineraryDay::ItineraryDay(int dayNumber, std::string notes) : dayNumber_(dayNumber), notes_(std::move(notes)) {}

void ItineraryDay::addActivity(const Activity& activity) {
    activities_.push_back(activity);
}

int ItineraryDay::totalMinutes() const {
    int minutes = 0;
    for (const Activity& activity : activities_) {
        minutes += activity.durationMinutes();
    }
    return minutes;
}

Money ItineraryDay::totalCost() const {
    Money total;
    for (const Activity& activity : activities_) {
        total = total.plus(activity.cost());
    }
    return total;
}

bool ItineraryDay::isOverloaded() const {
    return totalMinutes() > kMaxDayMinutes;
}
