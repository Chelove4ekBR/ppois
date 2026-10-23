#pragma once

#include <string>

class Address {
public:
    Address(std::string country, std::string city, std::string street, std::string postalCode);
    const std::string& country() const { return country_; }
    const std::string& city() const { return city_; }
    const std::string& street() const { return street_; }
    const std::string& postalCode() const { return postalCode_; }
    std::string fullText() const;
    bool isSameCity(const Address& other) const;
    bool isSameCountry(const Address& other) const;

private:
    std::string country_;
    std::string city_;
    std::string street_;
    std::string postalCode_;
};
