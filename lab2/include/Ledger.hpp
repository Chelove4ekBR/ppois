#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.hpp"
#include "LedgerEntry.hpp"
#include "Money.hpp"

class Ledger {
public:
    explicit Ledger(std::string name);
    const std::string& name() const { return name_; }
    const std::vector<LedgerEntry>& entries() const { return entries_; }
    void record(const LedgerEntry& entry);
    Money balance() const;
    Money totalIncome() const;
    Money totalExpenses() const;
    std::size_t entriesOn(const Date& date) const;

private:
    std::string name_;
    std::vector<LedgerEntry> entries_;
};
