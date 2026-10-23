#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Destination.hpp"
#include "Itinerary.hpp"
#include "Money.hpp"

class Tour {
public:
    Tour(int id, std::string name, Destination destination, int durationDays, Money basePrice, int capacity,
         Itinerary itinerary);
    virtual ~Tour() = default;
    int id() const { return id_; }
    const std::string& name() const { return name_; }
    const Destination& destination() const { return destination_; }
    int durationDays() const { return durationDays_; }
    const Money& basePrice() const { return basePrice_; }
    int capacity() const { return capacity_; }
    Itinerary& itinerary() { return itinerary_; }
    const Itinerary& itinerary() const { return itinerary_; }
    virtual Money pricePerPerson() const;
    Money priceFor(int participants) const;
    bool hasCapacityFor(int participants) const;
    bool isSuitableForSeason(const Date& date) const;
    virtual std::string describe() const;

private:
    int id_;
    std::string name_;
    Destination destination_;
    int durationDays_;
    Money basePrice_;
    int capacity_;
    Itinerary itinerary_;
};

using TourPtr = std::shared_ptr<Tour>;
