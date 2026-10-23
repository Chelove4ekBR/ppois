#include "TransportTicket.hpp"

#include <utility>

TransportTicket::TransportTicket(TransportPtr transport, CustomerPtr passenger)
    : transport_(std::move(transport)),
      passenger_(std::move(passenger)),
      seatNumber_(transport_->sellSeat()),
      price_(transport_->ticketPrice()) {}

bool TransportTicket::isForPassenger(int customerId) const
{
    return passenger_->customerId() == customerId;
}

void TransportTicket::cancel()
{
    transport_->refundSeat();
}

std::string TransportTicket::describe() const
{
    return passenger_->fullName() + ", seat " + std::to_string(seatNumber_) + ", " + price_.toString();
}
