#include "Address.hpp"

#include <utility>

Address::Address(std::string country, std::string city, std::string street, std::string postalCode)
    : country_(std::move(country)),
      city_(std::move(city)),
      street_(std::move(street)),
      postalCode_(std::move(postalCode)) {}

std::string Address::fullText() const {
    return street_ + ", " + city_ + " " + postalCode_ + ", " + country_;
}

bool Address::isSameCity(const Address& other) const {
    return isSameCountry(other) && city_ == other.city_;
}

bool Address::isSameCountry(const Address& other) const {
    return country_ == other.country_;
}
