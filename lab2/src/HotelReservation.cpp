#include "HotelReservation.hpp"

#include <utility>

HotelReservation::HotelReservation(int id, HotelPtr hotel, RoomPtr room, CustomerPtr guest, DateRange period,
                                   MealPlan mealPlan, int guestCount)
    : id_(id),
      hotel_(std::move(hotel)),
      room_(std::move(room)),
      guest_(std::move(guest)),
      period_(period),
      mealPlan_(std::move(mealPlan)),
      guestCount_(guestCount) {
    room_->reserve(period_);
}

int HotelReservation::nights() const {
    return period_.lengthInDays();
}

Money HotelReservation::totalCost() const {
    return room_->costFor(period_).plus(mealPlan_.costFor(guestCount_, nights()));
}

void HotelReservation::cancel() {
    room_->cancelReservation(period_);
}
