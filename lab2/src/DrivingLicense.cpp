#include "DrivingLicense.hpp"

#include <utility>

DrivingLicense::DrivingLicense(std::string number, std::string category, Date expiry)
    : number_(std::move(number)), category_(std::move(category)), expiry_(expiry) {}

bool DrivingLicense::allowsCategory(const std::string& requested) const {
    return category_.find(requested) != std::string::npos;
}

bool DrivingLicense::isValidOn(const Date& date) const {
    return !expiry_.isBefore(date);
}
