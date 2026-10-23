#pragma once

#include <string>

#include "Money.hpp"

class Activity {
public:
    Activity(std::string title, int durationMinutes, Money cost, bool outdoor);
    const std::string& title() const { return title_; }
    int durationMinutes() const { return durationMinutes_; }
    const Money& cost() const { return cost_; }
    bool outdoor() const { return outdoor_; }
    bool isLongerThan(int minutes) const;
    Money costForGroup(int people) const;
    std::string describe() const;

private:
    std::string title_;
    int durationMinutes_;
    Money cost_;
    bool outdoor_;
};
