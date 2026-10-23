#pragma once

#include <string>

class Date {
public:
    Date(int year, unsigned month, unsigned day);
    int year() const { return year_; }
    unsigned month() const { return month_; }
    unsigned day() const { return day_; }
    static Date fromDayNumber(long long dayNumber);
    long long dayNumber() const;
    Date addDays(int days) const;
    int daysUntil(const Date& other) const;
    bool isBefore(const Date& other) const;
    bool isSameDay(const Date& other) const;
    std::string toString() const;

private:
    int year_;
    unsigned month_;
    unsigned day_;
};
