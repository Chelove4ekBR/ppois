#include "Passport.hpp"

#include <algorithm>
#include <cctype>
#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr std::size_t kMinPassportLength = 6;
constexpr std::size_t kMaxPassportLength = 9;
constexpr int kPassportMarginDays = 180;

bool isAllAlphanumeric(const std::string& text) {
    return std::all_of(text.begin(), text.end(), [](unsigned char symbol) { return std::isalnum(symbol) != 0; });
}

}

Passport::Passport(std::string number, std::string issuingCountry, std::string holderName, Date expiry)
    : number_(std::move(number)),
      issuingCountry_(std::move(issuingCountry)),
      holderName_(std::move(holderName)),
      expiry_(expiry) {
    const bool lengthOk = number_.size() >= kMinPassportLength && number_.size() <= kMaxPassportLength;
    if (!lengthOk || !isAllAlphanumeric(number_)) {
        throw InvalidPassportException("Invalid passport number: " + number_);
    }
}

bool Passport::isExpiredOn(const Date& date) const {
    return expiry_.isBefore(date);
}

bool Passport::isValidForTrip(const DateRange& trip) const {
    return !expiry_.isBefore(trip.end().addDays(kPassportMarginDays));
}

void Passport::requireValidForTrip(const DateRange& trip) const {
    if (!isValidForTrip(trip)) {
        throw ExpiredPassportException("Passport " + number_ + " is not valid long enough for trip " +
                                       trip.toString());
    }
}
