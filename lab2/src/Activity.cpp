#include "Activity.hpp"

#include <utility>

Activity::Activity(std::string title, int durationMinutes, Money cost, bool outdoor)
    : title_(std::move(title)), durationMinutes_(durationMinutes), cost_(std::move(cost)), outdoor_(outdoor) {}

bool Activity::isLongerThan(int minutes) const {
    return durationMinutes_ > minutes;
}

Money Activity::costForGroup(int people) const {
    return cost_.times(people);
}

std::string Activity::describe() const {
    return title_ + " (" + std::to_string(durationMinutes_) + " min, " + cost_.toString() + ")";
}
