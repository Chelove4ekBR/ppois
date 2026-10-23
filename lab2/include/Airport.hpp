#pragma once

#include <string>

#include "City.hpp"

class Airport {
public:
    Airport(std::string code, std::string name, City city);
    const std::string& code() const { return code_; }
    const std::string& name() const { return name_; }
    const City& city() const { return city_; }
    std::string label() const;
    bool isInternationalTo(const Airport& other) const;

private:
    std::string code_;
    std::string name_;
    City city_;
};
