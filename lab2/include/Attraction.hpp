#pragma once

#include <string>

#include "Money.hpp"

class Attraction {
public:
    Attraction(std::string name, Money ticketPrice, int visitMinutes, bool indoor);
    const std::string& name() const { return name_; }
    const Money& ticketPrice() const { return ticketPrice_; }
    int visitMinutes() const { return visitMinutes_; }
    bool indoor() const { return indoor_; }
    bool isFree() const;
    Money costFor(int visitors) const;
    bool fitsInto(int availableMinutes) const;

private:
    std::string name_;
    Money ticketPrice_;
    int visitMinutes_;
    bool indoor_;
};
