#include "Visa.hpp"

#include <utility>

Visa::Visa(std::string country, std::string type, const DateRange &validity)
    : country_(std::move(country)), type_(std::move(type)), validity_(validity) {}

bool Visa::isActiveOn(const Date &date) const
{
    return validity_.contains(date);
}

bool Visa::covers(const std::string &countryCode, const DateRange &trip) const
{
    return country_ == countryCode && isActiveOn(trip.start()) && isActiveOn(trip.end());
}
