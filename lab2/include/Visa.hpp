#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"

class Visa {
public:
    Visa(std::string country, std::string type, DateRange validity);
    const std::string& country() const { return country_; }
    const std::string& type() const { return type_; }
    const DateRange& validity() const { return validity_; }
    bool isActiveOn(const Date& date) const;
    bool covers(const std::string& countryCode, const DateRange& trip) const;

private:
    std::string country_;
    std::string type_;
    DateRange validity_;
};
