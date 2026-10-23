#include "Airport.hpp"

#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr std::size_t kAirportCodeLength = 3;

}

Airport::Airport(std::string code, std::string name, City city)
    : code_(std::move(code)), name_(std::move(name)), city_(std::move(city)) {
    if (code_.size() != kAirportCodeLength) {
        throw TravelException("Airport code must have 3 letters: " + code_);
    }
}

std::string Airport::label() const {
    return code_ + " - " + name_ + " (" + city_.name() + ")";
}

bool Airport::isInternationalTo(const Airport& other) const {
    return !city_.isInCountry(other.city_.country());
}
