#include "SkiTour.hpp"

#include <utility>

namespace
{

    constexpr int kBeginnerDifficulty = 2;

}

SkiTour::SkiTour(Tour base, std::string resortName, Money skiPassPricePerDay, int difficulty)
    : Tour(std::move(base)),
      resortName_(std::move(resortName)),
      skiPassPricePerDay_(std::move(skiPassPricePerDay)),
      difficulty_(difficulty) {}

Money SkiTour::skiPassCost() const
{
    return skiPassPricePerDay_.times(durationDays());
}

bool SkiTour::isForBeginners() const
{
    return difficulty_ <= kBeginnerDifficulty;
}

Money SkiTour::pricePerPerson() const
{
    return basePrice().plus(skiPassCost());
}

std::string SkiTour::describe() const
{
    return "Ski tour: " + Tour::describe() + ", resort " + resortName_;
}
