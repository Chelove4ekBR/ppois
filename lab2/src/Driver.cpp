#include "Driver.hpp"

#include <utility>

#include "Exceptions.hpp"

namespace
{

    constexpr long long kMaxKmWithoutRest = 500;

}

Driver::Driver(int employeeId, Person person, Money salary, Date hireDate, DrivingLicense license)
    : Employee(employeeId, std::move(person), "Driver", std::move(salary), hireDate),
      license_(std::move(license)),
      totalKm_(0),
      kmSinceRest_(0) {}

bool Driver::canDrive(const std::string &category, const Date &date) const
{
    return license_.isValidOn(date) && license_.allowsCategory(category);
}

void Driver::logDistance(long long kilometers)
{
    if (kilometers < 0)
    {
        throw TravelException("Distance must not be negative");
    }
    totalKm_ += kilometers;
    kmSinceRest_ += kilometers;
}

bool Driver::needsRest() const
{
    return kmSinceRest_ >= kMaxKmWithoutRest;
}

void Driver::takeRest()
{
    kmSinceRest_ = 0;
}

std::string Driver::describeRole() const
{
    return "Driver " + fullName() + " (license " + license_.category() + ")";
}
