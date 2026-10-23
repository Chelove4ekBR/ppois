#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"

class Passport {
public:
    Passport(std::string number, std::string issuingCountry, std::string holderName, Date expiry);
    const std::string& number() const { return number_; }
    const std::string& issuingCountry() const { return issuingCountry_; }
    const std::string& holderName() const { return holderName_; }
    const Date& expiry() const { return expiry_; }
    bool isExpiredOn(const Date& date) const;
    bool isValidForTrip(const DateRange& trip) const;
    void requireValidForTrip(const DateRange& trip) const;

private:
    std::string number_;
    std::string issuingCountry_;
    std::string holderName_;
    Date expiry_;
};
