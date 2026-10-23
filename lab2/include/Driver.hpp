#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "DrivingLicense.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

class Driver : public Employee {
public:
    Driver(int employeeId, Person person, Money salary, Date hireDate, DrivingLicense license);
    const DrivingLicense& license() const { return license_; }
    long long totalKm() const { return totalKm_; }
    long long kmSinceRest() const { return kmSinceRest_; }
    bool canDrive(const std::string& category, const Date& date) const;
    void logDistance(long long kilometers);
    bool needsRest() const;
    void takeRest();
    std::string describeRole() const override;

private:
    DrivingLicense license_;
    long long totalKm_;
    long long kmSinceRest_;
};

using DriverPtr = std::shared_ptr<Driver>;
