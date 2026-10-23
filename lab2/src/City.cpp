#include "City.hpp"

#include <utility>

City::City(std::string name, Country country, int utcOffsetHours)
    : name_(std::move(name)), country_(std::move(country)), utcOffsetHours_(utcOffsetHours) {}

std::string City::label() const {
    return name_ + ", " + country_.name();
}

bool City::isInCountry(const Country& other) const {
    return country_.matchesCode(other.isoCode());
}

int City::timeDifferenceHours(const City& other) const {
    return other.utcOffsetHours_ - utcOffsetHours_;
}
