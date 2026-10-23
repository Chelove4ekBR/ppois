#include "Flight.hpp"

#include <utility>

namespace {

constexpr int kInternationalTaxPercent = 12;
constexpr long long kExcessLuggageFeePerKgCents = 1500;

}

Flight::Flight(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string flightNumber,
               Airport origin, Airport destination, Date departureDate, int luggageKgIncluded)
    : Transport(id, std::move(carrierName), capacity, std::move(pricePerSeat)),
      flightNumber_(std::move(flightNumber)),
      origin_(std::move(origin)),
      destination_(std::move(destination)),
      departureDate_(departureDate),
      luggageKgIncluded_(luggageKgIncluded) {}

bool Flight::isInternational() const {
    return origin_.isInternationalTo(destination_);
}

Money Flight::luggageFee(int luggageKg) const {
    const int excessKg = luggageKg > luggageKgIncluded_ ? luggageKg - luggageKgIncluded_ : 0;
    return Money(kExcessLuggageFeePerKgCents * excessKg, pricePerSeat().currency());
}

Money Flight::ticketPrice() const {
    const Money base = Transport::ticketPrice();
    return isInternational() ? base.plus(base.percentOf(kInternationalTaxPercent)) : base;
}

std::string Flight::describe() const {
    return "Flight " + flightNumber_ + " " + origin_.code() + "->" + destination_.code() + " on " +
           departureDate_.toString();
}
