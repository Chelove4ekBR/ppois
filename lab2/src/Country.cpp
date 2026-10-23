#include "Country.hpp"

#include <utility>

Country::Country(std::string name, std::string isoCode, std::string currency, bool visaRequired)
    : name_(std::move(name)), isoCode_(std::move(isoCode)), currency_(std::move(currency)), visaRequired_(visaRequired) {}

bool Country::matchesCode(const std::string& code) const {
    return isoCode_ == code;
}

std::string Country::describe() const {
    const std::string visaNote = visaRequired_ ? "visa required" : "no visa";
    return name_ + " (" + isoCode_ + ", " + currency_ + ", " + visaNote + ")";
}
