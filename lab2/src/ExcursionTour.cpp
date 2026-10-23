#include "ExcursionTour.hpp"

#include <utility>

namespace
{

    constexpr int kEasyWalkKm = 5;

}

ExcursionTour::ExcursionTour(Tour base, TourGuidePtr guide, std::string language, int walkingKm)
    : Tour(std::move(base)), guide_(std::move(guide)), language_(std::move(language)), walkingKm_(walkingKm) {}

bool ExcursionTour::guideSpeaksLanguage() const
{
    return guide_->speaks(language_);
}

bool ExcursionTour::isEasy() const
{
    return walkingKm_ <= kEasyWalkKm;
}

Money ExcursionTour::pricePerPerson() const
{
    const Money guideShare = guide_->costForDays(durationDays()).dividedBy(capacity());
    return basePrice().plus(guideShare);
}

std::string ExcursionTour::describe() const
{
    return "Excursion: " + Tour::describe() + ", guide " + guide_->fullName() + " (" + language_ + ")";
}
