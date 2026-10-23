#pragma once

#include <string>

#include "Airport.hpp"
#include "Date.hpp"
#include "Money.hpp"
#include "Transport.hpp"

class Flight : public Transport {
public:
    Flight(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string flightNumber,
           Airport origin, Airport destination, Date departureDate, int luggageKgIncluded);
    const std::string& flightNumber() const { return flightNumber_; }
    const Airport& origin() const { return origin_; }
    const Airport& destination() const { return destination_; }
    const Date& departureDate() const { return departureDate_; }
    int luggageKgIncluded() const { return luggageKgIncluded_; }
    bool isInternational() const;
    Money luggageFee(int luggageKg) const;
    Money ticketPrice() const override;
    std::string describe() const override;

private:
    std::string flightNumber_;
    Airport origin_;
    Airport destination_;
    Date departureDate_;
    int luggageKgIncluded_;
};
