#include "Transport.hpp"

#include <utility>

#include "Exceptions.hpp"

Transport::Transport(int id, std::string carrierName, int capacity, Money pricePerSeat)
    : id_(id),
      carrierName_(std::move(carrierName)),
      capacity_(capacity),
      soldSeats_(0),
      pricePerSeat_(std::move(pricePerSeat)) {}

int Transport::freeSeats() const
{
    return capacity_ - soldSeats_;
}

bool Transport::isFull() const
{
    return freeSeats() <= 0;
}

int Transport::sellSeat()
{
    if (isFull())
    {
        throw TourFullException("No free seats on " + carrierName_);
    }
    ++soldSeats_;
    return soldSeats_;
}

void Transport::refundSeat()
{
    if (soldSeats_ > 0)
    {
        --soldSeats_;
    }
}

Money Transport::ticketPrice() const
{
    return pricePerSeat_;
}

std::string Transport::describe() const
{
    return carrierName_ + " #" + std::to_string(id_) + ", free seats " + std::to_string(freeSeats());
}
