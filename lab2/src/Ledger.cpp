#include "Ledger.hpp"

#include <algorithm>
#include <utility>

Ledger::Ledger(std::string name) : name_(std::move(name)) {}

void Ledger::record(const LedgerEntry& entry) {
    entries_.push_back(entry);
}

Money Ledger::balance() const {
    Money total;
    for (const LedgerEntry& entry : entries_) {
        total = total.plus(entry.signedAmount());
    }
    return total;
}

Money Ledger::totalIncome() const {
    Money total;
    for (const LedgerEntry& entry : entries_) {
        total = entry.income() ? total.plus(entry.amount()) : total;
    }
    return total;
}

Money Ledger::totalExpenses() const {
    return totalIncome().minus(balance());
}

std::size_t Ledger::entriesOn(const Date& date) const {
    return static_cast<std::size_t>(std::count_if(
        entries_.begin(), entries_.end(), [&](const LedgerEntry& entry) { return entry.date().isSameDay(date); }));
}
