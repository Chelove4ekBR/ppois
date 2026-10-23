#include "Person.hpp"

#include <utility>

namespace {

constexpr int kAdultAge = 18;

}

Person::Person(std::string firstName, std::string lastName, Date birthDate, ContactInfo contact, Address address)
    : firstName_(std::move(firstName)),
      lastName_(std::move(lastName)),
      birthDate_(birthDate),
      contact_(std::move(contact)),
      address_(std::move(address)) {}

std::string Person::fullName() const {
    return firstName_ + " " + lastName_;
}

int Person::ageOn(const Date& date) const {
    int age = date.year() - birthDate_.year();
    const bool birthdayPassed = date.month() > birthDate_.month() ||
                                (date.month() == birthDate_.month() && date.day() >= birthDate_.day());
    return birthdayPassed ? age : age - 1;
}

bool Person::isAdultOn(const Date& date) const {
    return ageOn(date) >= kAdultAge;
}
