#include "LedgerEntry.hpp"

#include <utility>

LedgerEntry::LedgerEntry(std::string description, Money amount, Date date, bool income)
    : description_(std::move(description)), amount_(std::move(amount)), date_(date), income_(income) {}

Money LedgerEntry::signedAmount() const {
    return income_ ? amount_ : amount_.times(-1);
}

std::string LedgerEntry::describe() const {
    return date_.toString() + " " + description_ + ": " + signedAmount().toString();
}
