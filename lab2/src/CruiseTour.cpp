#include "CruiseTour.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr int kInsideCabinPercent = 100;
constexpr int kOceanCabinPercent = 130;
constexpr int kSuiteCabinPercent = 200;

}

CruiseTour::CruiseTour(Tour base, ShipPtr ship, std::string cabinCategory, std::vector<City> ports)
    : Tour(std::move(base)),
      ship_(std::move(ship)),
      cabinCategory_(std::move(cabinCategory)),
      ports_(std::move(ports)) {}

int CruiseTour::cabinMultiplierPercent() const {
    if (cabinCategory_ == "Inside") {
        return kInsideCabinPercent;
    }
    if (cabinCategory_ == "Ocean") {
        return kOceanCabinPercent;
    }
    if (cabinCategory_ == "Suite") {
        return kSuiteCabinPercent;
    }
    throw TravelException("Unknown cabin category: " + cabinCategory_);
}

std::size_t CruiseTour::portCount() const {
    return ports_.size();
}

bool CruiseTour::visitsCity(const std::string& cityName) const {
    return std::any_of(ports_.begin(), ports_.end(), [&](const City& port) { return port.name() == cityName; });
}

Money CruiseTour::pricePerPerson() const {
    return basePrice().percentOf(cabinMultiplierPercent());
}

std::string CruiseTour::describe() const {
    return "Cruise: " + Tour::describe() + ", " + ship_->describe() + ", cabin " + cabinCategory_;
}
