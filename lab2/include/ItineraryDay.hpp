#pragma once

#include <string>
#include <vector>

#include "Activity.hpp"
#include "Money.hpp"

class ItineraryDay {
public:
    ItineraryDay(int dayNumber, std::string notes);
    int dayNumber() const { return dayNumber_; }
    const std::string& notes() const { return notes_; }
    const std::vector<Activity>& activities() const { return activities_; }
    void addActivity(const Activity& activity);
    int totalMinutes() const;
    Money totalCost() const;
    bool isOverloaded() const;

private:
    int dayNumber_;
    std::string notes_;
    std::vector<Activity> activities_;
};
