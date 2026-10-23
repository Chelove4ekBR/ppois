#pragma once

#include <string>

#include "Date.hpp"

class DrivingLicense {
public:
    DrivingLicense(std::string number, std::string category, Date expiry);
    const std::string& number() const { return number_; }
    const std::string& category() const { return category_; }
    const Date& expiry() const { return expiry_; }
    bool allowsCategory(const std::string& requested) const;
    bool isValidOn(const Date& date) const;

private:
    std::string number_;
    std::string category_;
    Date expiry_;
};
