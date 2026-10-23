#include "Transfer.hpp"

#include <utility>

#include "Exceptions.hpp"

Transfer::Transfer(Address from, Address to, BusPtr bus, Date pickupDate, Money pricePerPassenger)
    : from_(std::move(from)),
      to_(std::move(to)),
      bus_(std::move(bus)),
      pickupDate_(pickupDate),
      pricePerPassenger_(std::move(pricePerPassenger)) {}

bool Transfer::isIntraCity() const
{
    return from_.isSameCity(to_);
}

Money Transfer::priceFor(int passengers) const
{
    if (passengers > bus_->capacity())
    {
        throw TourFullException("Transfer bus cannot carry " + std::to_string(passengers) + " passengers");
    }
    return pricePerPassenger_.times(passengers);
}

std::string Transfer::describe() const
{
    return "Transfer " + from_.city() + " -> " + to_.city() + " on " + pickupDate_.toString();
}
