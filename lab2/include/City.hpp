#pragma once

#include <string>

#include "Country.hpp"

class City {
public:
    City(std::string name, Country country, int utcOffsetHours);
    const std::string& name() const { return name_; }
    const Country& country() const { return country_; }
    int utcOffsetHours() const { return utcOffsetHours_; }
    std::string label() const;
    bool isInCountry(const Country& other) const;
    int timeDifferenceHours(const City& other) const;

private:
    std::string name_;
    Country country_;
    int utcOffsetHours_;
};
