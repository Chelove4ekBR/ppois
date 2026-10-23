#include "Ship.hpp"

#include <utility>

Ship::Ship(std::string name, int cabins, int decks, int passengerCapacity)
    : name_(std::move(name)), cabins_(cabins), decks_(decks), passengerCapacity_(passengerCapacity) {}

int Ship::cabinsPerDeck() const {
    return cabins_ / decks_;
}

bool Ship::canHost(int passengers) const {
    return passengers <= passengerCapacity_;
}

std::string Ship::describe() const {
    return "Ship " + name_ + ", " + std::to_string(cabins_) + " cabins on " + std::to_string(decks_) + " decks";
}
