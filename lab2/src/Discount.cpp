#include "Discount.hpp"

#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr int kMaxDiscountPercent = 50;

}

Discount::Discount(std::string code, int percent, DateRange validity, int usageLimit)
    : code_(std::move(code)), percent_(percent), validity_(validity), usageLimit_(usageLimit), timesUsed_(0) {
    if (percent_ <= 0 || percent_ > kMaxDiscountPercent) {
        throw InvalidDiscountException("Discount percent out of range: " + std::to_string(percent_));
    }
}

bool Discount::isValidOn(const Date& date) const {
    return validity_.contains(date) && remainingUses() > 0;
}

Money Discount::apply(const Money& price, const Date& date) {
    if (!isValidOn(date)) {
        throw InvalidDiscountException("Discount " + code_ + " is not valid on " + date.toString());
    }
    ++timesUsed_;
    return price.minus(price.percentOf(percent_));
}

int Discount::remainingUses() const {
    return usageLimit_ - timesUsed_;
}
