#pragma once

#include <string>

#include "Money.hpp"
#include "Tour.hpp"

class SkiTour : public Tour {
public:
    SkiTour(Tour base, std::string resortName, Money skiPassPricePerDay, int difficulty);
    const std::string& resortName() const { return resortName_; }
    const Money& skiPassPricePerDay() const { return skiPassPricePerDay_; }
    int difficulty() const { return difficulty_; }
    Money skiPassCost() const;
    bool isForBeginners() const;
    Money pricePerPerson() const override;
    std::string describe() const override;

private:
    std::string resortName_;
    Money skiPassPricePerDay_;
    int difficulty_;
};
