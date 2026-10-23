#pragma once

#include <string>
#include <vector>

#include "Attraction.hpp"
#include "City.hpp"
#include "Date.hpp"
#include "Money.hpp"

class Destination {
public:
    Destination(City city, std::string description, std::vector<unsigned> bestMonths);
    const City& city() const { return city_; }
    const std::string& description() const { return description_; }
    const std::vector<Attraction>& attractions() const { return attractions_; }
    const std::vector<unsigned>& bestMonths() const { return bestMonths_; }
    void addAttraction(const Attraction& attraction);
    bool isBestSeason(const Date& date) const;
    Money totalAttractionCost() const;
    const Attraction& cheapestAttraction() const;
    std::vector<Attraction> attractionsWithin(int minutes) const;

private:
    City city_;
    std::string description_;
    std::vector<Attraction> attractions_;
    std::vector<unsigned> bestMonths_;
};
