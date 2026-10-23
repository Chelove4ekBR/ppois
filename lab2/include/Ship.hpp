#pragma once

#include <memory>
#include <string>

class Ship {
public:
    Ship(std::string name, int cabins, int decks, int passengerCapacity);
    const std::string& name() const { return name_; }
    int cabins() const { return cabins_; }
    int decks() const { return decks_; }
    int passengerCapacity() const { return passengerCapacity_; }
    int cabinsPerDeck() const;
    bool canHost(int passengers) const;
    std::string describe() const;

private:
    std::string name_;
    int cabins_;
    int decks_;
    int passengerCapacity_;
};

using ShipPtr = std::shared_ptr<Ship>;
