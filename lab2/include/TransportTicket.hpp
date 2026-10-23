#pragma once

#include <string>

#include "Customer.hpp"
#include "Money.hpp"
#include "Transport.hpp"

class TransportTicket {
public:
    TransportTicket(TransportPtr transport, CustomerPtr passenger);
    const TransportPtr& transport() const { return transport_; }
    const CustomerPtr& passenger() const { return passenger_; }
    int seatNumber() const { return seatNumber_; }
    const Money& price() const { return price_; }
    bool isForPassenger(int customerId) const;
    void cancel();
    std::string describe() const;

private:
    TransportPtr transport_;
    CustomerPtr passenger_;
    int seatNumber_;
    Money price_;
};
