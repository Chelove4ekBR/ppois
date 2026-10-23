#include "Date.hpp"

#include <cstdio>
#include <ctime>

#include "Exceptions.hpp"

namespace
{

    constexpr std::size_t kDateBufferSize = 16;

    long long dateToDayNumber(int year, unsigned month, unsigned day)
    {
        int y = year;
        int m = static_cast<int>(month);
        int d = static_cast<int>(day);

        if (m < 3)
        {
            --y;
            m += 12;
        }
        long long era = (y >= 0 ? y : y - 399) / 400;
        int yoe = y - static_cast<int>(era * 400);
        int doy = (153 * (m - 3) + 2) / 5 + d - 1;
        int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        long long ratadie = era * 146097LL + doe;
        return ratadie - 719163LL;
    }

}

Date::Date(int year, unsigned month, unsigned day) : year_(year), month_(month), day_(day)
{

    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = static_cast<int>(month) - 1;
    t.tm_mday = static_cast<int>(day);
    t.tm_isdst = -1;
    time_t epoch = mktime(&t);
    if (epoch == static_cast<time_t>(-1) ||
        t.tm_year != year - 1900 ||
        t.tm_mon != static_cast<int>(month) - 1 ||
        t.tm_mday != static_cast<int>(day))
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

    long long z = dayNumber + 719163LL;
    long long era = (z >= 0 ? z : z - 146096) / 146097;
    int doe = static_cast<int>(z - era * 146097);
    int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int y = yoe + static_cast<int>(era * 400);
    int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    int mp = (5 * doy + 2) / 153;
    int d = doy - (153 * mp + 2) / 5 + 1;
    int m = mp < 10 ? mp + 3 : mp - 9;
    if (m <= 2)
        ++y;
    return Date(y, static_cast<unsigned>(m), static_cast<unsigned>(d));
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
    char buffer[kDateBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%04d-%02u-%02u", year_, month_, day_);
    return buffer;
}
