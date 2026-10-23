#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

class Discount {
public:
    Discount(std::string code, int percent, DateRange validity, int usageLimit);
    const std::string& code() const { return code_; }
    int percent() const { return percent_; }
    const DateRange& validity() const { return validity_; }
    int usageLimit() const { return usageLimit_; }
    int timesUsed() const { return timesUsed_; }
    bool isValidOn(const Date& date) const;
    Money apply(const Money& price, const Date& date);
    int remainingUses() const;

private:
    std::string code_;
    int percent_;
    DateRange validity_;
    int usageLimit_;
    int timesUsed_;
};
