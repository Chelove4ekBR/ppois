#pragma once

#include <string>

#include "Money.hpp"

class MealPlan {
public:
    MealPlan(std::string name, Money pricePerPersonPerDay, int mealsPerDay);
    const std::string& name() const { return name_; }
    const Money& pricePerPersonPerDay() const { return pricePerPersonPerDay_; }
    int mealsPerDay() const { return mealsPerDay_; }
    Money costFor(int guests, int nights) const;
    bool isFullBoard() const;
    std::string describe() const;

private:
    std::string name_;
    Money pricePerPersonPerDay_;
    int mealsPerDay_;
};
