#include "MealPlan.hpp"

#include <utility>

namespace
{

    constexpr int kFullBoardMeals = 3;

}

MealPlan::MealPlan(std::string name, Money pricePerPersonPerDay, int mealsPerDay)
    : name_(std::move(name)), pricePerPersonPerDay_(std::move(pricePerPersonPerDay)), mealsPerDay_(mealsPerDay) {}

Money MealPlan::costFor(int guests, int nights) const
{
    return pricePerPersonPerDay_.times(guests * nights);
}

bool MealPlan::isFullBoard() const
{
    return mealsPerDay_ >= kFullBoardMeals;
}

std::string MealPlan::describe() const
{
    return name_ + ": " + std::to_string(mealsPerDay_) + " meals/day, " + pricePerPersonPerDay_.toString();
}
