#pragma once

#include <string>

#include "Money.hpp"

class LoyaltyAccount {
public:
    LoyaltyAccount();
    int points() const { return points_; }
    const Money& lifetimeSpent() const { return lifetimeSpent_; }
    void earnPoints(const Money& spent);
    void redeemPoints(int amount);
    std::string tier() const;
    int discountPercent() const;

private:
    int points_;
    Money lifetimeSpent_;
};
