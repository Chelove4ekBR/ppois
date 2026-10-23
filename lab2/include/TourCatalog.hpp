#pragma once

#include <string>
#include <vector>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"
#include "Tour.hpp"
#include "TourDeparture.hpp"

class TourCatalog {
public:
    TourCatalog();
    const std::vector<TourPtr>& tours() const { return tours_; }
    const std::vector<DeparturePtr>& departures() const { return departures_; }
    void addTour(const TourPtr& tour);
    DeparturePtr addDeparture(const TourPtr& tour, const DateRange& period);
    TourPtr findTour(int tourId) const;
    DeparturePtr findDeparture(int departureId) const;
    std::vector<TourPtr> findByCity(const std::string& cityName) const;
    std::vector<TourPtr> findAffordable(const Money& maxPrice) const;
    std::vector<DeparturePtr> availableDepartures(const Date& from) const;

private:
    std::vector<TourPtr> tours_;
    std::vector<DeparturePtr> departures_;
};
