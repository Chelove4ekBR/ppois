#pragma once

#include <string>

#include "Date.hpp"
#include "Money.hpp"

class LedgerEntry {
public:
    LedgerEntry(std::string description, Money amount, Date date, bool income);
    const std::string& description() const { return description_; }
    const Money& amount() const { return amount_; }
    const Date& date() const { return date_; }
    bool income() const { return income_; }
    Money signedAmount() const;
    std::string describe() const;

private:
    std::string description_;
    Money amount_;
    Date date_;
    bool income_;
};
