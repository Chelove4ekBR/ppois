#pragma once

#include <string>
#include <vector>

#include "City.hpp"
#include "ItineraryDay.hpp"
#include "Money.hpp"

class Itinerary {
public:
    Itinerary(std::string title, City startCity);
    const std::string& title() const { return title_; }
    const City& startCity() const { return startCity_; }
    const std::vector<ItineraryDay>& days() const { return days_; }
    void addDay(const ItineraryDay& day);
    int totalDays() const;
    Money totalCost() const;
    const ItineraryDay& busiestDay() const;

private:
    std::string title_;
    City startCity_;
    std::vector<ItineraryDay> days_;
};
