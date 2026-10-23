#pragma once

#include <string>

#include "Address.hpp"
#include "Bus.hpp"
#include "Date.hpp"
#include "Money.hpp"

class Transfer {
public:
    Transfer(Address from, Address to, BusPtr bus, Date pickupDate, Money pricePerPassenger);
    const Address& from() const { return from_; }
    const Address& to() const { return to_; }
    const BusPtr& bus() const { return bus_; }
    const Date& pickupDate() const { return pickupDate_; }
    const Money& pricePerPassenger() const { return pricePerPassenger_; }
    bool isIntraCity() const;
    Money priceFor(int passengers) const;
    std::string describe() const;

private:
    Address from_;
    Address to_;
    BusPtr bus_;
    Date pickupDate_;
    Money pricePerPassenger_;
};
