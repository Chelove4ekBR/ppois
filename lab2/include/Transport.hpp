#pragma once

#include <memory>
#include <string>

#include "Money.hpp"

class Transport {
public:
    Transport(int id, std::string carrierName, int capacity, Money pricePerSeat);
    virtual ~Transport() = default;
    int id() const { return id_; }
    const std::string& carrierName() const { return carrierName_; }
    int capacity() const { return capacity_; }
    int soldSeats() const { return soldSeats_; }
    const Money& pricePerSeat() const { return pricePerSeat_; }
    int freeSeats() const;
    bool isFull() const;
    int sellSeat();
    void refundSeat();
    virtual Money ticketPrice() const;
    virtual std::string describe() const;

private:
    int id_;
    std::string carrierName_;
    int capacity_;
    int soldSeats_;
    Money pricePerSeat_;
};

using TransportPtr = std::shared_ptr<Transport>;
