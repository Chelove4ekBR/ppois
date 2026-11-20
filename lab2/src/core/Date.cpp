#include "core/Date.hpp"

#include <cstdio>
#include <ctime>

#include "core/Exceptions.hpp"

namespace
{

    constexpr std::size_t kDateBufferSize = 16;

    long long dateToDayNumber(int year, unsigned month, unsigned day)
    {
        int adjustedYear = year;
        int adjustedMonth = static_cast<int>(month);
        int adjustedDay = static_cast<int>(day);

        if (adjustedMonth < 3)
        {
            --adjustedYear;
            adjustedMonth += 12;
        }
        long long era = (adjustedYear >= 0 ? adjustedYear : adjustedYear - 399) / 400;
        int yearOfEra = adjustedYear - static_cast<int>(era * 400);
        int dayOfYear = (153 * (adjustedMonth - 3) + 2) / 5 + adjustedDay - 1;
        int dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
        long long ratadie = era * 146097LL + dayOfEra;
        return ratadie - 719163LL;
    }

}

Date::Date(int year, unsigned month, unsigned day) : year_(year), month_(month), day_(day)
{

    struct tm timeStruct = {};
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon = static_cast<int>(month) - 1;
    timeStruct.tm_mday = static_cast<int>(day);
    timeStruct.tm_isdst = -1;
    time_t epoch = mktime(&timeStruct);
    if (epoch == static_cast<time_t>(-1) ||
        timeStruct.tm_year != year - 1900 ||
        timeStruct.tm_mon != static_cast<int>(month) - 1 ||
        timeStruct.tm_mday != static_cast<int>(day))
    {
        throw InvalidDateException("Invalid calendar date: " + std::to_string(year) + "-" +
                                   std::to_string(month) + "-" + std::to_string(day));
    }
}

long long Date::dayNumber() const
{
    return dateToDayNumber(year_, month_, day_);
}

Date Date::fromDayNumber(long long dayNumber)
{

    long long daysSinceEpoch = dayNumber + 719163LL;
    long long era = (daysSinceEpoch >= 0 ? daysSinceEpoch : daysSinceEpoch - 146096) / 146097;
    int dayOfEra = static_cast<int>(daysSinceEpoch - era * 146097);
    int yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365;
    int calculatedYear = yearOfEra + static_cast<int>(era * 400);
    int dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100);
    int monthPosition = (5 * dayOfYear + 2) / 153;
    int calculatedDay = dayOfYear - (153 * monthPosition + 2) / 5 + 1;
    int calculatedMonth = monthPosition < 10 ? monthPosition + 3 : monthPosition - 9;
    if (calculatedMonth <= 2)
        ++calculatedYear;
    return Date(calculatedYear, static_cast<unsigned>(calculatedMonth), static_cast<unsigned>(calculatedDay));
}

Date Date::addDays(int days) const
{
    return fromDayNumber(dayNumber() + days);
}

int Date::daysUntil(const Date &other) const
{
    return static_cast<int>(other.dayNumber() - dayNumber());
}

bool Date::isBefore(const Date &other) const
{
    return dayNumber() < other.dayNumber();
}

bool Date::isSameDay(const Date &other) const
{
    return dayNumber() == other.dayNumber();
}

std::string Date::toString() const
{
    char formattedDate[kDateBufferSize];
    std::snprintf(formattedDate, sizeof(formattedDate), "%04d-%02u-%02u", year_, month_, day_);
    return formattedDate;
}
