#pragma once

#include <string>

#include "Money.hpp"
#include "Tour.hpp"
#include "TourGuide.hpp"

class ExcursionTour : public Tour {
public:
    ExcursionTour(Tour base, TourGuidePtr guide, std::string language, int walkingKm);
    const TourGuidePtr& guide() const { return guide_; }
    const std::string& language() const { return language_; }
    int walkingKm() const { return walkingKm_; }
    bool guideSpeaksLanguage() const;
    bool isEasy() const;
    Money pricePerPerson() const override;
    std::string describe() const override;

private:
    TourGuidePtr guide_;
    std::string language_;
    int walkingKm_;
};
