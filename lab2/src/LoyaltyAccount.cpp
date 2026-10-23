#include "LoyaltyAccount.hpp"

#include "Exceptions.hpp"

namespace
{

    constexpr long long kCentsPerPoint = 1000;
    constexpr int kSilverPoints = 500;
    constexpr int kGoldPoints = 2000;
    constexpr int kSilverDiscountPercent = 3;
    constexpr int kGoldDiscountPercent = 7;

}

LoyaltyAccount::LoyaltyAccount() : points_(0), lifetimeSpent_() {}

void LoyaltyAccount::earnPoints(const Money &spent)
{
    lifetimeSpent_ = lifetimeSpent_.plus(spent);
    points_ += static_cast<int>(spent.cents() / kCentsPerPoint);
}

void LoyaltyAccount::redeemPoints(int amount)
{
    if (amount > points_)
    {
        throw PaymentException("Not enough loyalty points: requested " + std::to_string(amount) + ", available " +
                               std::to_string(points_));
    }
    points_ -= amount;
}

std::string LoyaltyAccount::tier() const
{
    if (points_ >= kGoldPoints)
    {
        return "Gold";
    }
    return points_ >= kSilverPoints ? "Silver" : "Bronze";
}

int LoyaltyAccount::discountPercent() const
{
    const std::string currentTier = tier();
    if (currentTier == "Gold")
    {
        return kGoldDiscountPercent;
    }
    return currentTier == "Silver" ? kSilverDiscountPercent : 0;
}
