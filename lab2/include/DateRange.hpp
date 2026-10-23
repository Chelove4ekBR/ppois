#pragma once

#include <string>

#include "Date.hpp"

class DateRange {
public:
    DateRange(Date start, Date end);
    const Date& start() const { return start_; }
    const Date& end() const { return end_; }
    int lengthInDays() const;
    bool contains(const Date& date) const;
    bool overlaps(const DateRange& other) const;
    std::string toString() const;

private:
    Date start_;
    Date end_;
};
