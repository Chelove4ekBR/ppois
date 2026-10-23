#include "Customer.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

Customer::Customer(int customerId, Person person, Passport passport)
    : Person(std::move(person)), customerId_(customerId), passport_(std::move(passport)) {}

void Customer::addVisa(const Visa& visa) {
    visas_.push_back(visa);
}

bool Customer::hasVisaFor(const std::string& countryCode, const DateRange& trip) const {
    return std::any_of(visas_.begin(), visas_.end(),
                       [&](const Visa& visa) { return visa.covers(countryCode, trip); });
}

void Customer::ensureCanEnter(const Country& country, const DateRange& trip) const {
    passport_.requireValidForTrip(trip);
    if (country.visaRequired() && !hasVisaFor(country.isoCode(), trip)) {
        throw VisaRequiredException("Visa for " + country.name() + " is required for " + fullName());
    }
}
