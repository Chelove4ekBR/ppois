#include "DateRange.hpp"

#include "Exceptions.hpp"

DateRange::DateRange(Date start, Date end) : start_(start), end_(end)
{
    if (end_.isBefore(start_))
    {
        throw InvalidDateException("Range end " + end_.toString() + " is before start " + start_.toString());
    }
}

int DateRange::lengthInDays() const
{
    return start_.daysUntil(end_);
}

bool DateRange::contains(const Date &date) const
{
    return !date.isBefore(start_) && !end_.isBefore(date);
}

bool DateRange::overlaps(const DateRange &other) const
{
    return start_.isBefore(other.end_) && other.start_.isBefore(end_);
}

std::string DateRange::toString() const
{
    return start_.toString() + " .. " + end_.toString();
}
