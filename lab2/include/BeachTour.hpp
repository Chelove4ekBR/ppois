#pragma once

#include <string>

#include "Hotel.hpp"
#include "Money.hpp"
#include "Tour.hpp"

class BeachTour : public Tour {
public:
    BeachTour(Tour base, std::string beachName, HotelPtr hotel, bool snorkelingIncluded);
    const std::string& beachName() const { return beachName_; }
    const HotelPtr& hotel() const { return hotel_; }
    bool snorkelingIncluded() const { return snorkelingIncluded_; }
    bool hasHotelWithStars(int minimumStars) const;
    Money pricePerPerson() const override;
    std::string describe() const override;

private:
    std::string beachName_;
    HotelPtr hotel_;
    bool snorkelingIncluded_;
};
