#pragma once

#include "Customer.hpp"
#include "DateRange.hpp"
#include "Hotel.hpp"
#include "MealPlan.hpp"
#include "Money.hpp"
#include "Room.hpp"

class HotelReservation {
public:
    HotelReservation(int id, HotelPtr hotel, RoomPtr room, CustomerPtr guest, DateRange period, MealPlan mealPlan,
                     int guestCount);
    int id() const { return id_; }
    const HotelPtr& hotel() const { return hotel_; }
    const RoomPtr& room() const { return room_; }
    const CustomerPtr& guest() const { return guest_; }
    const DateRange& period() const { return period_; }
    const MealPlan& mealPlan() const { return mealPlan_; }
    int guestCount() const { return guestCount_; }
    int nights() const;
    Money totalCost() const;
    void cancel();

private:
    int id_;
    HotelPtr hotel_;
    RoomPtr room_;
    CustomerPtr guest_;
    DateRange period_;
    MealPlan mealPlan_;
    int guestCount_;
};
