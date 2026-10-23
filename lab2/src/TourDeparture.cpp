#include "TourDeparture.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

TourDeparture::TourDeparture(int id, TourPtr tour, DateRange period)
    : id_(id), tour_(std::move(tour)), period_(period), bookedSeats_(0) {}

int TourDeparture::freeSeats() const {
    return tour_->capacity() - bookedSeats_;
}

void TourDeparture::reserveSeats(int seats) {
    if (seats > freeSeats()) {
        throw TourFullException("Only " + std::to_string(freeSeats()) + " seats left on departure " +
                                std::to_string(id_));
    }
    bookedSeats_ += seats;
}

void TourDeparture::releaseSeats(int seats) {
    bookedSeats_ = std::max(0, bookedSeats_ - seats);
}

bool TourDeparture::isSoldOut() const {
    return freeSeats() == 0;
}

bool TourDeparture::startsWithin(const Date& today, int days) const {
    const int daysToStart = today.daysUntil(period_.start());
    return daysToStart >= 0 && daysToStart <= days;
}

bool TourDeparture::isFinished(const Date& today) const {
    return period_.end().isBefore(today);
}
