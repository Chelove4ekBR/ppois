#include "Bus.hpp"

#include <utility>

Bus::Bus(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string plateNumber, DriverPtr driver,
         bool hasToilet)
    : Transport(id, std::move(carrierName), capacity, std::move(pricePerSeat)),
      plateNumber_(std::move(plateNumber)),
      driver_(std::move(driver)),
      hasToilet_(hasToilet) {}

bool Bus::isDriverQualified(const Date& date) const {
    return driver_->canDrive("D", date);
}

std::string Bus::describe() const {
    return "Bus " + plateNumber_ + " driven by " + driver_->fullName();
}
