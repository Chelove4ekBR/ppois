#pragma once

#include <string>

#include "Money.hpp"
#include "Transport.hpp"

class Train : public Transport {
public:
    Train(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string trainNumber, int carriages,
          bool hasSleeper);
    const std::string& trainNumber() const { return trainNumber_; }
    int carriages() const { return carriages_; }
    bool hasSleeper() const { return hasSleeper_; }
    int seatsPerCarriage() const;
    Money ticketPrice() const override;
    std::string describe() const override;

private:
    std::string trainNumber_;
    int carriages_;
    bool hasSleeper_;
};
