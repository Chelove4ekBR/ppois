#include "TourCatalog.hpp"

#include <algorithm>
#include <iterator>

#include "Exceptions.hpp"

TourCatalog::TourCatalog() = default;

void TourCatalog::addTour(const TourPtr& tour) {
    tours_.push_back(tour);
}

DeparturePtr TourCatalog::addDeparture(const TourPtr& tour, const DateRange& period) {
    const int departureId = static_cast<int>(departures_.size()) + 1;
    DeparturePtr departure = std::make_shared<TourDeparture>(departureId, tour, period);
    departures_.push_back(departure);
    return departure;
}

TourPtr TourCatalog::findTour(int tourId) const {
    for (const TourPtr& tour : tours_) {
        if (tour->id() == tourId) {
            return tour;
        }
    }
    throw TourNotFoundException("Tour " + std::to_string(tourId) + " not found");
}

DeparturePtr TourCatalog::findDeparture(int departureId) const {
    for (const DeparturePtr& departure : departures_) {
        if (departure->id() == departureId) {
            return departure;
        }
    }
    throw TourNotFoundException("Departure " + std::to_string(departureId) + " not found");
}

std::vector<TourPtr> TourCatalog::findByCity(const std::string& cityName) const {
    std::vector<TourPtr> result;
    std::copy_if(tours_.begin(), tours_.end(), std::back_inserter(result),
                 [&](const TourPtr& tour) { return tour->destination().city().name() == cityName; });
    return result;
}

std::vector<TourPtr> TourCatalog::findAffordable(const Money& maxPrice) const {
    std::vector<TourPtr> result;
    std::copy_if(tours_.begin(), tours_.end(), std::back_inserter(result),
                 [&](const TourPtr& tour) { return !tour->pricePerPerson().isGreaterThan(maxPrice); });
    return result;
}

std::vector<DeparturePtr> TourCatalog::availableDepartures(const Date& from) const {
    std::vector<DeparturePtr> result;
    std::copy_if(departures_.begin(), departures_.end(), std::back_inserter(result), [&](const DeparturePtr& departure) {
        return !departure->period().start().isBefore(from) && !departure->isSoldOut();
    });
    return result;
}
