#include "BeachTour.hpp"

#include <utility>

namespace {

constexpr long long kSnorkelSurchargeCents = 5000;

}

BeachTour::BeachTour(Tour base, std::string beachName, HotelPtr hotel, bool snorkelingIncluded)
    : Tour(std::move(base)),
      beachName_(std::move(beachName)),
      hotel_(std::move(hotel)),
      snorkelingIncluded_(snorkelingIncluded) {}

bool BeachTour::hasHotelWithStars(int minimumStars) const {
    return hotel_->stars() >= minimumStars;
}

Money BeachTour::pricePerPerson() const {
    const Money surcharge(snorkelingIncluded_ ? kSnorkelSurchargeCents : 0, basePrice().currency());
    return basePrice().plus(surcharge);
}

std::string BeachTour::describe() const {
    return "Beach tour: " + Tour::describe() + ", beach " + beachName_ + ", hotel " + hotel_->name();
}
