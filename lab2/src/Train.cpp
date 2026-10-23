#include "Train.hpp"

#include <utility>

namespace {

constexpr int kSleeperSurchargePercent = 25;

}

Train::Train(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string trainNumber, int carriages,
             bool hasSleeper)
    : Transport(id, std::move(carrierName), capacity, std::move(pricePerSeat)),
      trainNumber_(std::move(trainNumber)),
      carriages_(carriages),
      hasSleeper_(hasSleeper) {}

int Train::seatsPerCarriage() const {
    return capacity() / carriages_;
}

Money Train::ticketPrice() const {
    const Money base = Transport::ticketPrice();
    return hasSleeper_ ? base.plus(base.percentOf(kSleeperSurchargePercent)) : base;
}

std::string Train::describe() const {
    return "Train " + trainNumber_ + ", " + std::to_string(carriages_) + " carriages";
}
