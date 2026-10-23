#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Driver.hpp"
#include "Money.hpp"
#include "Transport.hpp"

class Bus : public Transport {
public:
    Bus(int id, std::string carrierName, int capacity, Money pricePerSeat, std::string plateNumber, DriverPtr driver,
        bool hasToilet);
    const std::string& plateNumber() const { return plateNumber_; }
    const DriverPtr& driver() const { return driver_; }
    bool hasToilet() const { return hasToilet_; }
    bool isDriverQualified(const Date& date) const;
    std::string describe() const override;

private:
    std::string plateNumber_;
    DriverPtr driver_;
    bool hasToilet_;
};

using BusPtr = std::shared_ptr<Bus>;
